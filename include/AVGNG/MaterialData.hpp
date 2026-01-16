#pragma once

#include <AVGNG/Texture.hpp>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <optional>

namespace ng::Graphics {

    class MaterialData
    {
        std::string name;
        std::vector<ng::Graphics::Texture*> textures;

    public:
        MaterialData();
        ~MaterialData();
        glm::vec3 Albedo;               // Albedo
        glm::vec3 Ambient;              // Ambient
        glm::vec3 Diffuse;              // Diffuse
        glm::vec3 Specular;             // Specular
        glm::vec3 Emissive;             // Emissive
        float IOR = 1.5f;					              // Index of Refraction
        float Shininess = 1.0f;                           // Shininess 0 = dull, 2000 = shiny, sharp reflections

        /// <summary>Sets a texture of a specific type (e.g., diffuse, specular)</summary>
        /// <details>If texture type already exists, it replaces it.
        /// If texture type does NOT already exist, it adds a new texture.</details>
        /// <param name="map_type">NG_TEXTURE_TYPE_</param>
        /// <param name="texture">Texture</param>
        void SetTexture(const char* texture_type, Texture* texture);

		Texture* FindTexture(const char* texture_type);

    };

}