#pragma once

#include <string>
#include <glm/vec3.hpp>

namespace ng::Graphics {

        struct Material
        {
            std::string name;
            glm::vec3 Ka = glm::vec3(1.0f); // Ambient
            glm::vec3 Kd = glm::vec3(1.0f); // Diffuse
            glm::vec3 Ks = glm::vec3(0.0f); // Specular
            float Ns = 1.0f;               // Shininess

            unsigned int diffuseTexID = 0; // OpenGL texture handle
        };
    
}