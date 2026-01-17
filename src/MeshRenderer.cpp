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
                ImGui::SliderFloat("Index of Refraction", &matData->IOR, .0f, 1.0f);
                ImGui::SliderFloat("Shininess", &matData->Shininess, 0.0f, 1.0f);
                ImGui::SliderFloat("Opacity", &matData->Opacity, 0.0f, 1.0f); // new
            }

        }
        else {
            Debug::Log(WARN, "Inspected GameObject '%s': Mesh is missing material data", owner->name.c_str());
        }

       
    }

    void MeshRenderer::Save(nlohmann::json& j, int componentIndex)
    {
        MaterialData* meshMat = mesh->GetMaterial()->GetMaterialData();
        Shader* meshShader = mesh->GetMaterial()->GetShader();
        // Save mesh data
        j["meshRenderer"]["path"] = mesh->filepath;

        Debug::Log(DEBUG, "Saving MeshRenderer %p...", mesh);
        
      // Save material properties
      j["meshRenderer"]["material"]["mat_albedo_color"] = {meshMat->Albedo.x, meshMat->Albedo.y, meshMat->Albedo.z};
      j["meshRenderer"]["material"]["mat_ambient_color"] = { meshMat->Ambient.x, meshMat->Ambient.y, meshMat->Ambient.z };
      j["meshRenderer"]["material"]["mat_diffuse_color"] = { meshMat->Diffuse.x, meshMat->Diffuse.y, meshMat->Diffuse.z };
      j["meshRenderer"]["material"]["mat_specular_color"] = { meshMat->Specular.x, meshMat->Specular.y, meshMat->Specular.z };
      j["meshRenderer"]["material"]["mat_emissive_color"] = { meshMat->Emissive.x, meshMat->Emissive.y, meshMat->Emissive.z };
      j["meshRenderer"]["material"]["mat_shininess"] = meshMat->Shininess;
      j["meshRenderer"]["material"]["mat_ior"] = meshMat->IOR;
      j["meshRenderer"]["material"]["mat_opacity"] = meshMat->Opacity;
      
      // Find all existing texture paths
      Texture* diffuseTex = meshMat->FindTexture(TextureType::DIFFUSE);
      Texture* specularTex = meshMat->FindTexture(TextureType::SPECULAR);
      Texture* emissiveTex = meshMat->FindTexture(TextureType::EMISSIVE);
      Texture* normalTex = meshMat->FindTexture(TextureType::NORMAL);
      Texture* alphaTex = meshMat->FindTexture(TextureType::ALPHA);

      // Save texture data
      j["meshRenderer"]["material"]["mat_specular_texture_path"] = specularTex ? specularTex->path : "";
      j["meshRenderer"]["material"]["mat_emissive_texture_path"] = emissiveTex ? emissiveTex->path : "";
      j["meshRenderer"]["material"]["mat_normal_texture_path"] = normalTex ? normalTex->path : "";
      j["meshRenderer"]["material"]["mat_alpha_texture_path"] = alphaTex ? alphaTex->path : "";

      // Save shader data
      j["meshRenderer"]["shader"]["vertex_shader_path"] = meshShader->vertex_shader_path;
      j["meshRenderer"]["shader"]["fragment_shader_path"] = meshShader->fragment_shader_path;

      Debug::Log(DEBUG, "Finished saving MeshRenderer %p", mesh);

    }


}

