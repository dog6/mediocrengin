#include <glad/glad.h>
#include "AVGNG/Graphics/MeshRenderer.hpp"
#include "AVGNG/Assets/AssimpObjLoader.hpp"
#include "AVGNG/Graphics/Shader.hpp"
#include "AVGNG/Graphics/ShaderLoader.hpp"
#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Assets/JsonUtils.hpp"
#include "AVGNG/Graphics/Mesh.hpp"
#include "AVGNG/Assets/TextureLoader.hpp"
#include "AVGNG/Editor/FileDialog.hpp"
#include <string>

#include "AVGNG/Assets/ScopedTimer.hpp"

#include <imgui/imgui.h>

using namespace ng::Core;

namespace ng::Graphics {

    bool MeshRenderer::ValidateMesh()
    {
        bool result = true;
        int errNum = 0;
        // Early validation checks
        if (!this->mesh) {
            Debug::Log(LogLevel::ERROR, "Mesh is null in MeshRenderer::Draw");
            result = false;
            errNum -= 1;
        }

        if (this->mesh->indices.empty()) {
            Debug::Log(LogLevel::WARN, "Mesh has no indices");
            result = false;
            errNum -= 1;
        }

        if (!this->mesh->material) {
            Debug::Log(LogLevel::WARN, "Mesh has no material assigned");
            result = false;
            errNum -= 1;
        }

        if (!result) Debug::Log(ERROR, "MeshRenderer failed to validate mesh, error: %d", errNum);

        return result;

    }

    namespace {

        bool ParseTextureType(const std::string& name, ng::Graphics::TextureType& out)
        {
            if (name == "diffuse")  { out = TextureType::DIFFUSE;  return true; }
            if (name == "specular") { out = TextureType::SPECULAR; return true; }
            if (name == "normal")   { out = TextureType::NORMAL;   return true; }
            if (name == "emissive") { out = TextureType::EMISSIVE; return true; }
            if (name == "alpha")    { out = TextureType::ALPHA;    return true; }
            if (name == "metallic") { out = TextureType::METALLIC; return true; }
            return false;
        }

        // Sends tiling and offset for all texture slots.
        // Always call this. An unset GLSL uniform is 0, and a tiling of 0 breaks the texture.
        void UploadTextureTransforms(Shader& shader, MaterialData& matData)
        {
            struct Slot {
                TextureType type;
                const char* tilingName;
                const char* offsetName;
            };
            
            static const Slot slots[] = {
                { TextureType::DIFFUSE,  "diffuseMapTiling",  "diffuseMapOffset"  },
                { TextureType::SPECULAR, "specularMapTiling", "specularMapOffset" },
                { TextureType::NORMAL,   "normalMapTiling",   "normalMapOffset"   },
                { TextureType::EMISSIVE, "emissiveMapTiling", "emissiveMapOffset" },
                { TextureType::ALPHA,    "alphaMapTiling",    "alphaMapOffset"    },
                { TextureType::METALLIC, "metallicMapTiling", "metallicMapOffset" },
            };

            for (const Slot& slot : slots) {
                const TextureTransform& t = matData.GetTextureTransform(slot.type);
                shader.SetVec2(slot.tilingName, t.tiling);
                shader.SetVec2(slot.offsetName, t.offset);
            }
        }

        void DrawTextureSlot(const char* label, TextureType type, MaterialData& matData)
        {
            Texture* tex = matData.FindTexture(type);

            ImGui::TextUnformatted(label);

            const ImVec2 previewSize(48.0f, 48.0f);
            if (tex != nullptr)
                ImGui::Image((ImTextureID)(intptr_t)tex->id, previewSize);
            else
                ImGui::Dummy(previewSize);

            ImGui::SameLine();
            ImGui::BeginGroup();

            if (tex != nullptr)
                ImGui::Text("ID %u | %d x %d", (unsigned int)tex->id, tex->width, tex->height);
            else
                ImGui::TextDisabled("none");

            // "..." button: select a file
            if (ImGui::Button("...")) {
                std::string path;
                if (ng::Editor::FileDialog::OpenImage(path)) {
                    Texture* newTex = ng::Assets::TextureLoader::LoadFromFile(path);
                    if (newTex != nullptr)
                        matData.SetTexture(type, newTex); // MaterialData owns it now
                    else
                        Debug::Log(LogLevel::ERROR, "Could not load texture '%s'", path.c_str());
                }
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Select a texture file");

            // "Clear" button
            ImGui::SameLine();
            ImGui::BeginDisabled(tex == nullptr);
            if (ImGui::Button("Clear")) matData.RemoveTexture(type);
            ImGui::EndDisabled();

            // "Settings" button: tiling and offset
            ImGui::SameLine();
            if (ImGui::Button("Settings")) ImGui::OpenPopup("TextureSettings");

            if (ImGui::BeginPopup("TextureSettings")) {
                TextureTransform& t = matData.GetTextureTransform(type);

                ImGui::Text("%s map settings", label);
                ImGui::Separator();

                ImGui::DragFloat2("Tiling", &t.tiling.x, 0.01f, 0.001f, 100.0f);
                ImGui::DragFloat2("Offset", &t.offset.x, 0.01f);

                if (ImGui::Button("Reset (1:1)"))
                    t = TextureTransform{};

                ImGui::EndPopup();
            }

            ImGui::EndGroup();
        }

    } // anonymous namespace

    bool MeshRenderer::SetTextureTiling(const std::string& typeName, float x, float y)
    {
        TextureType type;
        Mesh* mesh = GetMesh();
        if (!ParseTextureType(typeName, type) || mesh == nullptr || mesh->material == nullptr)
            return false;

        mesh->material->GetMaterialData()->GetTextureTransform(type).tiling = glm::vec2(x, y);
        return true;
    }

    bool MeshRenderer::SetTexture(const std::string& typeName, const std::string& path)
    {
        TextureType type;
        if (!ParseTextureType(typeName, type)) {
            Debug::Log(LogLevel::ERROR, "SetTexture: unknown texture type '%s'", typeName.c_str());
            return false;
        }

        Mesh* mesh = GetMesh();
        if (mesh == nullptr || mesh->material == nullptr) {
            Debug::Log(LogLevel::ERROR, "SetTexture: load a mesh before you set a texture");
            return false;
        }

        MaterialData* matData = mesh->material->GetMaterialData();
        if (matData == nullptr) return false;

        Texture* texture = ng::Assets::TextureLoader::LoadFromFile(path);
        if (texture == nullptr) return false; // TextureLoader logs the error

        matData->SetTexture(type, texture); // MaterialData owns the texture now
        return true;
    }

    // Runs every frame
    // An issue now is that we're drawing per object, and not clustering objects
    // together based on which shader they're using. We should use render queues in the future to save
    // performance
    void MeshRenderer::Draw(Camera& camera, Transform& transform)
    {
       if (!ValidateMesh()) return; // only draw valid mesh
        mesh->material->GetMaterialData()->ReleaseRetiredTextures();

       Debug::Log(VERBOSE, "Validated mesh, drawing");
       
        // 1. Get the shader assigned to the mesh material
        Shader* shader = this->mesh->material->GetShader();
        if (!shader) {
            Debug::Log(LogLevel::WARN, "Mesh material has no valid shader assigned");
            return;
        }

        
        // Use shader program
        this->mesh->UseShader(camera, transform);
        UploadTextureTransforms(*shader, *mesh->material->GetMaterialData());
        Debug::Log(VERBOSE, "Binding VAO and drawing elements..");

        // 4. Bind VAO and draw
        glBindVertexArray(mesh->VAO);
        glDrawElements(mesh->drawMode, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        Debug::Log(VERBOSE, "Unbinding shader..");

        // 5. Unbind shader program (good practice)
        glUseProgram(0);
    }

    // Getters & Setters
    void MeshRenderer::SetMesh(Mesh* mesh)
    {
        this->mesh = mesh;
    }
    
    Mesh* MeshRenderer::GetMesh() { return this->mesh; }

    void MeshRenderer::LoadMesh(const char* objPath) 
    {
        // It's good practice to check for null if coming from Lua/C
        if (!objPath) {
            Debug::Log(ERROR, "MeshRenderer::LoadMesh -> objPath is null!");
            return;
        }

        Debug::Log(LOG, "MeshRenderer::LoadMesh -> %p", (void*)this);
        Debug::Log(LOG, " .OBJ Path: '%s'", objPath);

        // Load mesh using AssimpObjLoader
        Mesh* loadedMesh = ng::Assets::AssimpObjLoader::LoadObjAsMesh(objPath);

        if (loadedMesh == nullptr) {
            Debug::Log(ERROR, "Failed to load mesh from MeshRenderer with path '%s'.", objPath);
            return;
        }

        if (!loadedMesh->material) {
            Debug::Log(DEBUG, "Mesh had no material, creating default Material");
            loadedMesh->material = new ng::Graphics::Material();
        }

        Debug::Log(LOG, "Loaded Mesh '%s' for MeshRenderer attached to GameObject: '%s'", 
                objPath, owner->name.c_str());

        this->SetMesh(loadedMesh);
    }

   namespace {
    // Calls PushID in the constructor and PopID in the destructor.
    // PopID then runs on each return path.
    struct IdScope {
        explicit IdScope(const void* id) { ImGui::PushID(id); }
        ~IdScope() { ImGui::PopID(); }
    };

    const ImVec4 ERROR_COLOR(1.0f, 0.3f, 0.3f, 1.0f);
}

void MeshRenderer::OnInspectorGUI() {

    IdScope idScope(this);

    ImGui::Text("MeshRenderer Component [%p]", (void*)this);

    ng::Graphics::Mesh* mesh = GetMesh();
    if (mesh == nullptr) {
        ImGui::TextColored(ERROR_COLOR, "No mesh assigned.");
        return;
    }

    ImGui::Text("Vertices: %zu", mesh->vertices.size());
    ImGui::Text("Indices: %zu", mesh->indices.size());
    ImGui::Text("Triangles: %zu", mesh->indices.size() / 3);

    MaterialData* matData = mesh->material ? mesh->material->GetMaterialData() : nullptr;
    if (matData == nullptr) {
        ImGui::TextColored(ERROR_COLOR, "This mesh has no material data.");
        return;
    }

    if (ImGui::CollapsingHeader("Material Properties", ImGuiTreeNodeFlags_DefaultOpen)) {

        ImGui::TextDisabled("Surface");
        ImGui::SliderFloat("Index of Refraction", &matData->IOR, 1.0f, 128.0f);
        ImGui::SliderFloat("Shininess", &matData->Shininess, 0.01f, 1.0f);
        ImGui::SliderFloat("Opacity", &matData->Opacity, 0.0f, 1.0f);
        ImGui::SliderFloat("Metallicness", &matData->Metallicness, 0.0f, 1.0f);

        ImGui::Separator();
        ImGui::TextDisabled("Colors");

        auto ColorControl = [](const char* label, auto& color) {
            ImGui::ColorEdit3(label, reinterpret_cast<float*>(&color), ImGuiColorEditFlags_NoAlpha);
        };

        ImGui::PushItemWidth(100.0f);
        ColorControl("Albedo Color",   matData->Albedo);
        ColorControl("Diffuse Color",  matData->Diffuse);
        ColorControl("Specular Color", matData->Specular);
        ColorControl("Emissive Color", matData->Emissive);
        ColorControl("Ambient Color",  matData->Ambient);
        ImGui::PopItemWidth();
    }

    if (ImGui::CollapsingHeader("Textures")) {

        struct TextureSlot { const char* name; TextureType type; };
        static const TextureSlot slots[] = {
            { "Diffuse",  TextureType::DIFFUSE  },
            { "Specular", TextureType::SPECULAR },
            { "Normal",   TextureType::NORMAL   },
            { "Emissive", TextureType::EMISSIVE },
            { "Alpha",    TextureType::ALPHA    },
            { "Metallic", TextureType::METALLIC },
        };

        for (const TextureSlot& slot : slots) {
            ImGui::PushID(slot.name);
            DrawTextureSlot(slot.name, slot.type, *matData);
            ImGui::Separator();
            ImGui::PopID();
        }
    }
}

    void MeshRenderer::Save(nlohmann::json& j)
    {
        nlohmann::json& mr = j["meshRenderer"];

        // A MeshRenderer without a mesh still round-trips (as an empty entry)
        mr["path"] = mesh ? mesh->filepath : "";
        if (!mesh) return;

        Debug::Log(DEBUG, "Saving MeshRenderer %p...", mesh);

        Material* material = mesh->GetMaterial();
        if (!material) {
            Debug::Log(WARN, "MeshRenderer mesh '%s' has no material, skipping material save", mesh->filepath.c_str());
            return;
        }

        MaterialData* meshMat = material->GetMaterialData();
        nlohmann::json& mat = mr["material"];

        // Save material properties
        mat["mat_albedo_color"] = ng::Assets::Vec3ToJson(meshMat->Albedo);
        mat["mat_ambient_color"] = ng::Assets::Vec3ToJson(meshMat->Ambient);
        mat["mat_diffuse_color"] = ng::Assets::Vec3ToJson(meshMat->Diffuse);
        mat["mat_specular_color"] = ng::Assets::Vec3ToJson(meshMat->Specular);
        mat["mat_emissive_color"] = ng::Assets::Vec3ToJson(meshMat->Emissive);
        mat["mat_shininess"] = meshMat->Shininess;
        mat["mat_ior"] = meshMat->IOR;
        mat["mat_opacity"] = meshMat->Opacity;

        // Find all existing texture paths
        Texture* diffuseTex = meshMat->FindTexture(TextureType::DIFFUSE);
        Texture* specularTex = meshMat->FindTexture(TextureType::SPECULAR);
        Texture* emissiveTex = meshMat->FindTexture(TextureType::EMISSIVE);
        Texture* normalTex = meshMat->FindTexture(TextureType::NORMAL);
        Texture* alphaTex = meshMat->FindTexture(TextureType::ALPHA);

        // Save texture data
        mat["mat_diffuse_texture_path"] = diffuseTex ? diffuseTex->path : "";
        mat["mat_specular_texture_path"] = specularTex ? specularTex->path : "";
        mat["mat_emissive_texture_path"] = emissiveTex ? emissiveTex->path : "";
        mat["mat_normal_texture_path"] = normalTex ? normalTex->path : "";
        mat["mat_alpha_texture_path"] = alphaTex ? alphaTex->path : "";

        // Save shader data
        Shader* meshShader = material->GetShader();
        if (meshShader) {
            mr["shader"]["vertex_shader_path"] = meshShader->vertex_shader_path;
            mr["shader"]["fragment_shader_path"] = meshShader->fragment_shader_path;
        }

        Debug::Log(DEBUG, "Finished saving MeshRenderer %p", mesh);

    }

    void MeshRenderer::Load(const nlohmann::json& j)
    {
        if (!j.contains("meshRenderer")) return;
        const nlohmann::json& mr = j.at("meshRenderer");

        std::string meshPath = mr.value("path", "");
        if (!meshPath.empty()) LoadMesh(meshPath.c_str()); // logs on failure

        if (!mesh) return;

        Material* material = mesh->GetMaterial();
        if (!material) return;

        // Restore material properties
        if (mr.contains("material")) {

            const nlohmann::json& mat = mr.at("material");
            MaterialData* matData = material->GetMaterialData();

            matData->Albedo = ng::Assets::ReadVec3(mat, "mat_albedo_color", matData->Albedo);
            matData->Ambient = ng::Assets::ReadVec3(mat, "mat_ambient_color", matData->Ambient);
            matData->Diffuse = ng::Assets::ReadVec3(mat, "mat_diffuse_color", matData->Diffuse);
            matData->Specular = ng::Assets::ReadVec3(mat, "mat_specular_color", matData->Specular);
            matData->Emissive = ng::Assets::ReadVec3(mat, "mat_emissive_color", matData->Emissive);
            matData->Shininess = mat.value("mat_shininess", matData->Shininess);
            matData->IOR = mat.value("mat_ior", matData->IOR);
            matData->Opacity = mat.value("mat_opacity", matData->Opacity);

            // TODO: Restore material textures from mat_*_texture_path entries
            // ^^ bad comment, no clue what this means. if you can figure it out in the future, god speed.
        }

        // Restore shader
        
        if (mr.contains("shader")) {
            const nlohmann::json& sh = mr.at("shader");
            std::string vertPath = sh.value("vertex_shader_path", "");
            std::string fragPath = sh.value("fragment_shader_path", "");

            if (!vertPath.empty() && !fragPath.empty()) {
                // cache key: both paths, so distinct programs don't collide
                std::string shaderName = vertPath + ";" + fragPath;
                Shader shader = ng::Assets::ShaderLoader::LoadShader(shaderName.c_str(), vertPath.c_str(), fragPath.c_str());
                material->SetShader(shader);
            }
        }

    }


}

