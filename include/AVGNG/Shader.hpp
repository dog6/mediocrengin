#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include <AVGNG/FileReader.hpp>

namespace ng::Graphics {

        class Shader {
        public:
            Shader(const char* vertexCode, const char* fragmentCode);
            ~Shader();
            unsigned int ID;

            static std::string ReadShaderFile(const char* shader_filePath);

            static Shader* LoadShader(const char* vertex_shader_filePath, const char* frag_shader_filePath);


            void Use();

            void SetMat4(const char* name, glm::mat4 matrix);

            void SetVec4(const char* name, glm::vec4 vec);

            void SetVec3(const char* name, glm::vec3 vec);

            void SetInt(const char* name, int v);

        };


}