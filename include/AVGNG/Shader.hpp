#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <AVGNG/FileReader.hpp>

namespace ng::Graphics {

        class Shader {

        public:
            Shader();
            ~Shader();
            unsigned int ID = 0; // default ID to 0

   
            void Build(const char* vertexCode, const char* fragmentCode);

            void Use();

            void SetMat4(const char* name, glm::mat4 matrix);

            void SetVec4(const char* name, glm::vec4 vec);

            void SetVec3(const char* name, glm::vec3 vec);

            void SetInt(const char* name, int v);

            void SetFloat(const char* name, float v);

            void SetBool(const char* name, bool v);

        };


}