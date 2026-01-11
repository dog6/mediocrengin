#include <AVGNG/MeshRenderer.hpp>

using namespace ng::Core;

namespace ng::Graphics {

    void MeshRenderer::Draw(Camera& camera, Transform& transform)
    {

        // Early validation checks
        if (!this->shader) {
            Debug::Log(LogLevel::ERROR, "Shader is null in MeshRenderer::Draw");
            return;
        }

        if (!this->mesh) {
            Debug::Log(LogLevel::ERROR, "Mesh is null in MeshRenderer::Draw");
            return;
        }

        if (this->mesh->indices.size() == 0) {
            Debug::Log(LogLevel::WARN, "Mesh has no indices");
            return;
        }
        
        // Use shader
        this->shader->Use();

        // Get matrices
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix(1280, 720);
        glm::mat4 model = transform.GetModelMatrix();

        // Set matrices
        this->shader->SetMat4("view", view);
        this->shader->SetMat4("projection", projection);
        this->shader->SetMat4("model", model);

        // Set material properties
        if (mesh->material) {
            shader->SetVec3("baseColor", mesh->material->Kd);
            shader->SetVec3("sunDirection", glm::normalize(glm::vec3(-0.3f, -1.0f, -0.5f)));
            shader->SetVec3("sunColor", glm::vec3(1.0f, 0.95f, 0.8f));
            shader->SetVec3("viewPos", camera.position);
        }
        else {
            Debug::Log(LogLevel::WARN, "No material assigned, using magenta");
            shader->SetVec3("baseColor", glm::vec3(1.0f, 0.0f, 1.0f));
        }

        // Bind and draw
        glBindVertexArray(this->mesh->VAO);

        // Bind texture if available
        if (this->mesh->material && this->mesh->material->diffuseTexID > 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, this->mesh->material->diffuseTexID);
            this->shader->SetInt("diffuseMap", 0);
        }
        else {
			Debug::Log(WARN, "Failed to bind texture to MeshRenderer %p", (void*)this);
        }

        // Draw the mesh
        glDrawElements(GL_TRIANGLES, (GLsizei)this->mesh->indices.size(), GL_UNSIGNED_INT, 0);

        // Cleanup
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


        Mesh* loadedMesh = ng::Assets::ObjFileParser::LoadObjFromFileAsMesh(objPath);
    
        if (loadedMesh == nullptr) {
            Debug::Log(ERROR, "Failed to load mesh from MeshRenderer with path '%s'.", objPath);
            return;
        }

        // loadedMesh != nullptr
        Debug::Log(LOG, "Loaded Mesh '%s' for MeshRenderer attached to GameObject: '%s'", objPath, owner->name.c_str());
        this->SetMesh(loadedMesh);

    }


    /// <summary>
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
        
    
    }

}