#include "AVGNG/Graphics/DebugDraw.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "AVGNG/Graphics/Shader.hpp"

namespace ng::Graphics {

    GLuint DebugDraw::VAO = 0;
    GLuint DebugDraw::VBO = 0;
    GLuint DebugDraw::shaderID = 0;


    void DebugDraw::Initialize()
    {
        if (VAO != 0)
            return;

        // ---------------------------------------------------------
        // Vertex shader
        // ---------------------------------------------------------

        const char* vertexShaderSource = R"(
            #version 330 core

            layout (location = 0) in vec3 aPos;

            uniform mat4 view;
            uniform mat4 projection;

            void main()
            {
                gl_Position = projection * view * vec4(aPos, 1.0);
            }
        )";


        // ---------------------------------------------------------
        // Fragment shader
        // ---------------------------------------------------------

        const char* fragmentShaderSource = R"(
            #version 330 core

            uniform vec3 color;

            out vec4 FragColor;

            void main()
            {
                FragColor = vec4(color, 1.0);
            }
        )";


        // ---------------------------------------------------------
        // Build shader manually
        // ---------------------------------------------------------

        Shader shader;
        shader.Build(vertexShaderSource, fragmentShaderSource);

        shaderID = shader.ID;


        // ---------------------------------------------------------
        // Create VAO / VBO
        // ---------------------------------------------------------

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Enough space for 24 vec3s = 12 lines.
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(glm::vec3) * 24,
            nullptr,
            GL_DYNAMIC_DRAW
        );

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec3),
            nullptr
        );

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }


    void DebugDraw::Box(
        ng::Graphics::Camera& cam,
        const ng::Core::AABB& box,
        const glm::vec3& color)
    {
        Initialize();

        const glm::vec3& min = box.min;
        const glm::vec3& max = box.max;


        // ---------------------------------------------------------
        // AABB corners
        // ---------------------------------------------------------

        glm::vec3 corners[8] = {
            // Bottom
            { min.x, min.y, min.z }, // 0
            { max.x, min.y, min.z }, // 1
            { max.x, min.y, max.z }, // 2
            { min.x, min.y, max.z }, // 3

            // Top
            { min.x, max.y, min.z }, // 4
            { max.x, max.y, min.z }, // 5
            { max.x, max.y, max.z }, // 6
            { min.x, max.y, max.z }  // 7
        };


        // ---------------------------------------------------------
        // 12 edges
        //
        // Each pair of vertices represents one GL_LINES segment.
        // ---------------------------------------------------------

        glm::vec3 lines[24] = {

            // Bottom
            corners[0], corners[1],
            corners[1], corners[2],
            corners[2], corners[3],
            corners[3], corners[0],

            // Top
            corners[4], corners[5],
            corners[5], corners[6],
            corners[6], corners[7],
            corners[7], corners[4],

            // Vertical
            corners[0], corners[4],
            corners[1], corners[5],
            corners[2], corners[6],
            corners[3], corners[7]
        };


        // ---------------------------------------------------------
        // Get current viewport dimensions
        // ---------------------------------------------------------

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        float width = static_cast<float>(viewport[2]);
        float height = static_cast<float>(viewport[3]);

        if (height <= 0.0f)
            return;


        // ---------------------------------------------------------
        // Create matrices
        // ---------------------------------------------------------

        glm::mat4 view = cam.GetViewMatrix();

        glm::mat4 projection =
            cam.GetProjectionMatrix(width, height);


        // ---------------------------------------------------------
        // Use shader
        // ---------------------------------------------------------

        glUseProgram(shaderID);


        GLint viewLocation =
            glGetUniformLocation(shaderID, "view");

        GLint projectionLocation =
            glGetUniformLocation(shaderID, "projection");

        GLint colorLocation =
            glGetUniformLocation(shaderID, "color");


        glUniformMatrix4fv(
            viewLocation,
            1,
            GL_FALSE,
            glm::value_ptr(view)
        );

        glUniformMatrix4fv(
            projectionLocation,
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );

        glUniform3fv(
            colorLocation,
            1,
            glm::value_ptr(color)
        );


        // ---------------------------------------------------------
        // Upload line vertices
        // ---------------------------------------------------------

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            sizeof(lines),
            lines
        );


        // ---------------------------------------------------------
        // Draw 12 lines
        // ---------------------------------------------------------

        // So all gizmos are drawn on top layer
        // GLboolean depthWasOn = glIsEnabled(GL_DEPTH_TEST);
        // glDisable(GL_DEPTH_TEST);

        // Draw gizmos
        glDrawArrays(GL_LINES, 0, 24);
        // if (depthWasOn) glEnable(GL_DEPTH_TEST);

        // ---------------------------------------------------------
        // Cleanup
        // ---------------------------------------------------------

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

}