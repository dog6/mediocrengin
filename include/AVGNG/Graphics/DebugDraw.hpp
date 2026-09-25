#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "AVGNG/Graphics/Camera.hpp"
#include "AVGNG/Core/collision/AABB.hpp"

namespace ng::Graphics {

    class DebugDraw {
    private:
        static GLuint VAO;
        static GLuint VBO;
        static GLuint shaderID;

        static void Initialize();

    public:
        // Draws the AABB as 12 lines.
        static void Box(
            ng::Graphics::Camera& cam,
            const ng::Core::AABB& box,
            const glm::vec3& color
        );
    };

}