#include <AVGNG/MeshRenderer.hpp>
#include <AVGNG/AssimpObjLoader.hpp>
#include <AVGNG/Shader.hpp>
#include <AVGNG/ShaderLoader.hpp>
#include <AVGNG/GameObject.hpp>
#include <imgui.h>

using namespace ng::Core;

namespace ng::Graphics {

    void MeshRenderer::Draw(Camera& camera, Transform& transform)
    {
        // Early validation checks

        if (!this->mesh) {
            Debug::Log(LogLevel::ERROR, "Mesh is null in MeshRenderer::Draw");
            return;
        }

        if (this->mesh->indices.size() == 0) {
            Debug::Log(LogLevel::WARN, "Mesh has no indices");
            return;
        }

        if (!this->mesh->material) {
            Debug::Log(LogLevel::WARN, "Mesh has no material assigned");
            return;
        }

        this->mesh->UseShader(camera, transform);

        // Use shader
        // 
        // Bind, Draw, Cleanup
        glBindVertexArray(mesh->VAO);
        glDrawElements(GL_TRIANGLES, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
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

   /* /// <summary>
    /// Loads a shader for this->mesh using given vertex and fragment shader path.
    /// </summary>
    /// <param name="vertShaderPath">path to shader.vert file</param>
    /// <param name="fragShaderPath">path to shader.frag file</param>
    void MeshRenderer::LoadShader(const char* shaderName, const char* vertShaderPath, const char* fragShaderPath) {

        Debug::Log(DEBUG, "LoadShader called on MeshRenderer %p", (void*)this);
        Debug::Log(DEBUG, "    Vertex: '%s'", vertShaderPath);
        Debug::Log(DEBUG, "    Fragment: '%s'", fragShaderPath);

        if (!std::filesystem::exists(vertShaderPath)) {
            Debug::Log(ERROR, "Failed to load vertex shader for MeshRenderer %p", (void*)this);
            Debug::Log(ERROR, "    Path: '%s'", vertShaderPath);
            return;
        }

        if (!std::filesystem::exists(fragShaderPath)) {
            Debug::Log(ERROR, "Failed to load fragment shader for MeshRenderer %p", (void*)this);
            Debug::Log(ERROR, "    Path: '%s'", fragShaderPath);
            return;
        }

        Shader* loadedShader = ng::Assets::ShaderLoader::LoadShader(shaderName, vertShaderPath, fragShaderPath);


        if (!loadedShader) {
            Debug::Log(ERROR, "Failed to load mesh shader.\n    Vertex Shader: '%s'\n    Fragment Shader: '%s'", vertShaderPath, fragShaderPath);
            this->shader = nullptr;
            return;
        }

        this->shader = loadedShader;
        Debug::Log(LOG, "Sucessfully loaded shader for MeshRenderer %p", (void*)this);
        Debug::Log(LOG, "    Vertex: '%s'", vertShaderPath);
        Debug::Log(LOG, "    Fragment: '%s'", fragShaderPath);
        
    
    }*/


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

        if (matData != nullptr) {

            if (ImGui::CollapsingHeader("Material Properties", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick)) {
                if (diffTex) ImGui::Text("Diffuse Map: %d", diffTex->id);
                if (specTex) ImGui::Text("Specular Map: %d", specTex->id);
                if (emissiveTex) ImGui::Text("Emissive Map: %d", emissiveTex->id);
                if (normTex) ImGui::Text("Normal Map: %d", normTex->id);
                if (alphaTex) ImGui::Text("Alpha Map: %d", alphaTex->id);

				ImGui::ColorPicker3("Albedo Color", (float*)&matData->Albedo, ImGuiColorEditFlags_NoAlpha);
                ImGui::ColorPicker3("Diffuse Color", (float*)&matData->Diffuse, ImGuiColorEditFlags_NoAlpha);
                ImGui::ColorPicker3("Specular Color", (float*)&matData->Specular, ImGuiColorEditFlags_NoAlpha);
                ImGui::ColorPicker3("Emissive Color", (float*)&matData->Emissive, ImGuiColorEditFlags_NoAlpha);
                ImGui::ColorPicker3("Ambient Color", (float*)&matData->Ambient, ImGuiColorEditFlags_NoAlpha);
                ImGui::SliderFloat("Index of Refraction", &matData->IOR, 1.0f, 3.0f);
                ImGui::SliderFloat("Shininess", &matData->Shininess, 0.0f, 2000.0f);
            }

        }
        else {
            Debug::Log(WARN, "Inspected GameObject '%s': Mesh is missing material data", owner->name.c_str());
        }

       
    }


}

