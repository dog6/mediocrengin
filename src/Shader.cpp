#include <AVGNG/Shader.hpp>

using namespace ng::Core;
using namespace ng::Assets;

namespace ng::Graphics {
    std::string Shader::ReadShaderFile(const char* shader_filePath)
    {

        std::ifstream file = FileReader::ReadFile(shader_filePath);
        if (!file.is_open()) {
            Debug::Log(LogLevel::ERROR, "Failed to load shader %s", shader_filePath);
            return "";
        }

        std::string vertShaderProgram;

        std::string line;
        while (std::getline(file, line)) {
            vertShaderProgram += line + "\n";
        }

        return vertShaderProgram;
    }

    Shader* Shader::LoadShader(const char* vertex_shader_filePath, const char* frag_shader_filePath)
    {

        Debug::Log(LogLevel::DEBUG, "Loading shaders:\n\t- Vertex: %s\n\t- Fragment: %s\n", vertex_shader_filePath, frag_shader_filePath);

        // Read vertex shader
        std::string vertShader = ReadShaderFile(vertex_shader_filePath);
        std::string fragShader = ReadShaderFile(frag_shader_filePath);

        Shader* result = new Shader(vertShader.c_str(), fragShader.c_str());
        return result;

    }


    Shader::Shader(const char* vertexCode, const char* fragmentCode)
    {
        {
            int success;
            char infoLog[512];

            // Compile vertex shader
            unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vertexCode, NULL);
            glCompileShader(vertex);

            // Check vertex shader
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                Debug::Log(LogLevel::ERROR, "ERROR: Vertex shader compilation failed\n%s\n", infoLog);
            }

            // Compile fragment shader
            unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fragmentCode, NULL);
            glCompileShader(fragment);

            // Check fragment shader
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                Debug::Log(LogLevel::ERROR, "ERROR: Fragment shader compilation failed\n%s", infoLog);
            }

            // Link program
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);

            // Check linking
            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(ID, 512, NULL, infoLog);
                Debug::Log(LogLevel::ERROR, "ERROR: Shader program linking failed\n%s\n", infoLog);
            }

            glDeleteShader(vertex);
            glDeleteShader(fragment);

            Debug::Log(LogLevel::DEBUG, "Shader created successfully! ID: %d\n", ID);
        }
    }

    Shader::~Shader()
    {
    }


    void Shader::Use()
    {
        glUseProgram(ID);
    }

    void Shader::SetMat4(const char* name, glm::mat4 matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void Shader::SetVec4(const char* name, glm::vec4 vec)
    {
        glUniform4f(glGetUniformLocation(ID, name), vec.x, vec.y, vec.z, vec.w);
    }

    void Shader::SetVec3(const char* name, glm::vec3 vec)
    {
        glUniform3f(glGetUniformLocation(ID, name), vec.x, vec.y, vec.z);
    }

    void Shader::SetInt(const char* name, int v)
    {
        glUniform1i(glGetUniformLocation(ID, name), v);
    }

}