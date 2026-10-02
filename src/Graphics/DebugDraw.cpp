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
        // Build shader manually
        // ---------------------------------------------------------
        const char* vertexShaderSource = R"(
            #version 330 core

            layout (location = 0) in vec3 aPos;

            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            void main()
            {
                gl_Position = projection * view * model * vec4(aPos, 1.0);
            }
        )";

        const char* fragmentShaderSource = R"(
            #version 330 core

            uniform vec3 color;

            out vec4 FragColor;

            void main()
            {
                FragColor = vec4(color, 1.0);
            }
        )";

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
    const glm::mat4& model,
    const glm::vec3& color)
{
    Initialize();

    // Unit cube: -0.5 to +0.5
    glm::vec3 lines[24] = {

        // Bottom
        { -0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },

        {  0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f,  0.5f },

        {  0.5f, -0.5f,  0.5f },
        { -0.5f, -0.5f,  0.5f },

        { -0.5f, -0.5f,  0.5f },
        { -0.5f, -0.5f, -0.5f },

        // Top
        { -0.5f,  0.5f, -0.5f },
        {  0.5f,  0.5f, -0.5f },

        {  0.5f,  0.5f, -0.5f },
        {  0.5f,  0.5f,  0.5f },

        {  0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },

        { -0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f, -0.5f },

        // Vertical
        { -0.5f, -0.5f, -0.5f },
        { -0.5f,  0.5f, -0.5f },

        {  0.5f, -0.5f, -0.5f },
        {  0.5f,  0.5f, -0.5f },

        {  0.5f, -0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },

        { -0.5f, -0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f }
    };

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    float width = static_cast<float>(viewport[2]);
    float height = static_cast<float>(viewport[3]);

    if (height <= 0.0f)
        return;

    glm::mat4 view = cam.GetViewMatrix();
    glm::mat4 projection = cam.GetProjectionMatrix(width, height);

    glUseProgram(shaderID);

    GLint modelLocation =
        glGetUniformLocation(shaderID, "model");

    GLint viewLocation =
        glGetUniformLocation(shaderID, "view");

    GLint projectionLocation =
        glGetUniformLocation(shaderID, "projection");

    GLint colorLocation =
        glGetUniformLocation(shaderID, "color");

    glUniformMatrix4fv(
        modelLocation,
        1,
        GL_FALSE,
        glm::value_ptr(model)
    );

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

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        sizeof(lines),
        lines
    );

    glDrawArrays(GL_LINES, 0, 24);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

}