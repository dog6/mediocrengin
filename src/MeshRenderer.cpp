#include <AVGNG/MeshRenderer.hpp>

using namespace ng::Core;

namespace ng::Graphics {

    void MeshRenderer::Draw(Camera& camera, Transform& transform)
    {

        // Early validation checks
        if (this->shader == nullptr) {
            Debug::Log(LogLevel::ERROR, "Shader is null in MeshRenderer::Draw");
            return;
        }

        if (this->mesh == nullptr) {
            Debug::Log(LogLevel::ERROR, "Mesh is null in MeshRenderer::Draw");
            return;
        }

        if (this->mesh->indices.size() == 0) {
            Debug::Log(LogLevel::WARN, "Mesh has no indices");
            return;
        }

        // Get matrices
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix(1280, 720);
        glm::mat4 model = transform.GetModelMatrix();

        // Use shader and set uniforms
        shader->Use();
        shader->SetMat4("view", view);
        shader->SetMat4("projection", projection);
        shader->SetMat4("model", model);

        // Set material properties
        if (mesh->material) {
            shader->SetVec3("baseColor", mesh->material->Kd);
        }
        else {
            Debug::Log(LogLevel::WARN, "No material assigned, using magenta");
            shader->SetVec3("baseColor", glm::vec3(1.0f, 0.0f, 1.0f));
        }

        // Bind and draw
        glBindVertexArray(mesh->VAO);

        // Bind texture if available
        if (mesh->material && mesh->material->diffuseTexID > 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mesh->material->diffuseTexID);
            shader->SetInt("diffuseMap", 0);
        }

        // Draw the mesh
        glDrawElements(GL_TRIANGLES, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0);

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
    void MeshRenderer::LoadShader(const char* vertShaderPath, const char* fragShaderPath) {

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

        Shader* loadedShader = ng::Assets::ShaderLoader::LoadShader(vertShaderPath, fragShaderPath);

        if (loadedShader == nullptr) {
            Debug::Log(ERROR, "Failed to load mesh shader.\n    Vertex Shader: '%s'\n    Fragment Shader: '%s'", vertShaderPath, fragShaderPath);
            return;
        }

        Debug::Log(LOG, "Sucessfully loaded shader for MeshRenderer %p", (void*)this);
        Debug::Log(LOG, "    Vertex: '%s'", vertShaderPath);
        Debug::Log(LOG, "    Fragment: '%s'", fragShaderPath);
        
        this->shader = loadedShader;
    
    }

}