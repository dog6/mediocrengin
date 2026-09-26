#include "AVGNG/Graphics/MeshRenderer.hpp"
#include "AVGNG/Assets/AssimpObjLoader.hpp"
#include "AVGNG/Graphics/Shader.hpp"
#include "AVGNG/Graphics/ShaderLoader.hpp"
#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Assets/JsonUtils.hpp"
#include "AVGNG/Graphics/Mesh.hpp"

#include <imgui/imgui.h>

using namespace ng::Core;

namespace ng::Graphics {

    void MeshRenderer::Draw(Camera& camera, Transform& transform)
    {
        // Early validation checks
        if (!this->mesh) {
            Debug::Log(LogLevel::ERROR, "Mesh is null in MeshRenderer::Draw");
            return;
        }

        if (this->mesh->indices.empty()) {
            Debug::Log(LogLevel::WARN, "Mesh has no indices");
            return;
        }

        if (!this->mesh->material) {
            Debug::Log(LogLevel::WARN, "Mesh has no material assigned");
            return;
        }

        // 1. Get the shader assigned to the mesh material
        Shader* shader = this->mesh->material->GetShader();
        if (!shader) {
            Debug::Log(LogLevel::WARN, "Mesh material has no valid shader assigned");
            return;
        }

        // 2. ACTIVATE THE SHADER PROGRAM ON THE GPU
        shader->Use(); // Ensures glUseProgram(shader->ID) is executed

        // 3. Upload uniforms (Model, View, Projection, Light, Material data)
        this->mesh->UseShader(camera, transform);

        // 4. Bind VAO and draw
        glBindVertexArray(mesh->VAO);
        glDrawElements(mesh->drawMode, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // 5. Unbind shader program (good practice)
        glUseProgram(0);
    }

    // Getters & Setters
    void MeshRenderer::SetMesh(Mesh* mesh)
    {
        this->mesh = mesh;
    }
    
    Mesh* MeshRenderer::GetMesh() { return this->mesh; }

    /// <summary>
    /// Loads a mesh for this->mesh using a given .obj file path
    /// </summary>
    /// <param name="objPath">.obj file path</param>
    void MeshRenderer::LoadMesh(const char* objPath)
    {
        Debug::Log(DEBUG, "LoadMesh called on MeshRenderer %p", (void*)this);
        Debug::Log(DEBUG, "    .OBJ Path: '%s'", objPath);

        // Load mesh using AssimpObjLoader
		Mesh* loadedMesh = ng::Assets::AssimpObjLoader::LoadObjAsMesh(objPath);
    
        if (loadedMesh == nullptr) {
            Debug::Log(ERROR, "Failed to load mesh from MeshRenderer with path '%s'.", objPath);
            return;
        }

        if (!loadedMesh->material) {
            Debug::Log(DEBUG, "Mesh had no material, creating default Material");
            loadedMesh->material = new ng::Graphics::Material();
            // Shader should  be set automatically by material to ShaderLoader::s_defaultShader
        }

        Debug::Log(LOG, "Loaded Mesh '%s' for MeshRenderer attached to GameObject: '%s'", objPath, owner->name.c_str());
        this->SetMesh(loadedMesh);

    }

    void MeshRenderer::OnInspectorGUI() {

        // Render imgui elements for MeshRenderer Component
        ng::Graphics::MeshRenderer* mr = static_cast<ng::Graphics::MeshRenderer*>(this);
        ng::Graphics::Mesh* mesh = mr->GetMesh();
        ImGui::Text("MeshRenderer Component [%p]", mr);

        if (mesh == nullptr) {
            ImGui::TextColored(ImColor(255, 0, 0), "No mesh assigned.");
            return;
        }

        ImGui::Text("Vertices: %d", (int)mesh->vertices.size());
        ImGui::Text("Indices: %d", (int)mesh->indices.size());

        MaterialData* matData = mesh->material->GetMaterialData();

		Texture* diffTex = matData->FindTexture(TextureType::DIFFUSE);
		Texture* specTex = matData->FindTexture(TextureType::SPECULAR);
		Texture* normTex = matData->FindTexture(TextureType::NORMAL);
		Texture* emissiveTex = matData->FindTexture(TextureType::EMISSIVE);
        Texture* alphaTex = matData->FindTexture(TextureType::ALPHA);
		Texture* metallicTex = matData->FindTexture(TextureType::METALLIC);

        if (matData != nullptr) {

            if (ImGui::CollapsingHeader("Material Properties", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick)) {

                // For testing, should be replaced with way to change texture per mesh
                // ImGui::Text("Diffuse Map: %s", diffTex ? to_string(diffTex->id).c_str() : "NULL");
                // ImGui::Text("Specular Map: %s", specTex ? to_string(specTex->id).c_str() : "NULL");
                // ImGui::Text("Emissive Map: %s", emissiveTex ? to_string(emissiveTex->id).c_str() : "NULL");
                // ImGui::Text("Normal Map: %s", normTex ? to_string(normTex->id).c_str() : "NULL");
                // ImGui::Text("Alpha Map: %s", alphaTex ? to_string(alphaTex->id).c_str() : "NULL");
                // ImGui::Text("Metallic Map: %s", alphaTex ? to_string(alphaTex->id).c_str() : "NULL");
                

                ImGui::SliderFloat("Index of Refraction", &matData->IOR, 1.0f, 128.0f);
                ImGui::SliderFloat("Shininess", &matData->Shininess, 0.01f, 1.0f);
                ImGui::SliderFloat("Opacity", &matData->Opacity, 0.0f, 1.0f); // new
                ImGui::SliderFloat("Metallicness", &matData->Metallicness, 0.0f, 1.0f); // new

                ImGui::PushItemWidth(100);
				ImGui::ColorEdit3("Albedo Color", (float*)&matData->Albedo, ImGuiColorEditFlags_NoAlpha);
                ImGui::ColorEdit3("Diffuse Color", (float*)&matData->Diffuse, ImGuiColorEditFlags_NoAlpha);
                ImGui::ColorEdit3("Specular Color", (float*)&matData->Specular, ImGuiColorEditFlags_NoAlpha);
                ImGui::ColorEdit3("Emissive Color", (float*)&matData->Emissive, ImGuiColorEditFlags_NoAlpha);
                ImGui::ColorEdit3("Ambient Color", (float*)&matData->Ambient, ImGuiColorEditFlags_NoAlpha);


            }

        }
        else {
            Debug::Log(WARN, "Inspected GameObject '%s': Mesh is missing material data", owner->name.c_str());
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

