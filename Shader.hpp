#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "FileReader.hpp"

namespace ng {
    namespace Graphics {

        class Shader {
        public:
            unsigned int ID;

            static std::string ReadShaderFile(const char* shader_filePath) {
                
                std::ifstream file = FileReader::ReadFile(shader_filePath);
                if (!file.is_open()) {
                    printf("Failed to load shader %s", shader_filePath);
                    return "";
                }

                std::string vertShaderProgram;

                std::string line;
                while (std::getline(file, line)) {
                    vertShaderProgram += line + "\n";
                }

                return vertShaderProgram;
            }

            static Shader* LoadShader(const char* vertex_shader_filePath, const char* frag_shader_filePath) {
                
                printf("Loading shaders:\n\t- Vertex: %s\n\t- Fragment: %s\n", vertex_shader_filePath, frag_shader_filePath);
                
                // Read vertex shader
                std::string vertShader = ReadShaderFile(vertex_shader_filePath);
                std::string fragShader = ReadShaderFile(frag_shader_filePath);

                Shader* result = new Shader(vertShader.c_str(), fragShader.c_str());
                return result;

            }

            Shader(const char* vertexCode, const char* fragmentCode) {
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
                    printf("ERROR: Vertex shader compilation failed\n%s\n", infoLog);
                }

                // Compile fragment shader
                unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(fragment, 1, &fragmentCode, NULL);
                glCompileShader(fragment);

                // Check fragment shader
                glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
                if (!success) {
                    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                    printf("ERROR: Fragment shader compilation failed\n%s\n", infoLog);
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
                    printf("ERROR: Shader program linking failed\n%s\n", infoLog);
                }

                glDeleteShader(vertex);
                glDeleteShader(fragment);

                printf("Shader created successfully! ID: %d\n", ID);
            }

            void Use() {
                glUseProgram(ID);
            }

            void SetMat4(const char* name, glm::mat4 matrix) {
                glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
            }

            void SetVec4(const char* name, glm::vec4 vec) {
                glUniform4f(glGetUniformLocation(ID, name), vec.x, vec.y, vec.z, vec.w);
            }

            void SetVec3(const char* name, glm::vec3 vec) {
                glUniform3f(glGetUniformLocation(ID, name), vec.x, vec.y, vec.z);
            }


        };

    }
}