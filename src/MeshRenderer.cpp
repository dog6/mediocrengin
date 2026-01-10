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

        if (mesh->indices.size() == 0) {
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



}