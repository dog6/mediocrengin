#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ng::Graphics {

class Camera {
private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float fov;
    glm::vec2 viewportSize{ 1280.0f, 720.0f }; // Default value. Update it when the viewport changes.

public:
    // Single constructor handling both default initialization and custom inputs
    Camera(const glm::vec3& startPosition = glm::vec3(0.0f, 3.0f, 5.0f),
           float initialFOV = 90.0f)
        : position(startPosition),
          target(0.0f, 0.0f, 0.0f),
          up(0.0f, 1.0f, 0.0f),
          fov(initialFOV) {}

    // View matrix
    glm::mat4 GetViewMatrix() const {
        return glm::lookAt(position, target, up);
    }

    // Call this function when the window or the viewport changes size.
    void SetViewportSize(float w, float h) { viewportSize = glm::vec2(w, h); }

    // Uses the stored size. Use this function in all draw code.
    glm::mat4 GetProjectionMatrix() const {
        return GetProjectionMatrix(viewportSize.x, viewportSize.y);
    }

    // Uses the size that you give. This function does not change the camera.
    glm::mat4 GetProjectionMatrix(float viewport_width, float viewport_height) const {
        if (viewport_height <= 0.0f) return glm::mat4(1.0f); // Guard against minimization
        const float aspectRatio = viewport_width / viewport_height;
        return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
    }

    // Getters
    const glm::vec3& GetPosition() const { return position; }
    const glm::vec3& GetTarget() const { return target; }
    const glm::vec3& GetUp() const { return up; }
    float GetFOV() const { return fov; }
    const glm::vec2& GetViewportSize() const { return viewportSize; }

    // Setters
    void SetPosition(const glm::vec3& pos) { this->position = pos; }
    void SetTarget(const glm::vec3& target) { this->target = target; }
    void SetUp(const glm::vec3& up) { this->up = glm::normalize(up); }
    void SetFOV(float newFov) { this->fov = newFov; }
};

} // namespace ng::Graphics