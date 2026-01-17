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
        bool hasDiffuseTexture = false;
		bool hasSpecularTexture = false;
		bool hasNormalTexture = false;
		bool hasEmissiveTexture = false;
		bool hasAlphaTexture = false;
    public:
        MaterialData();
        ~MaterialData();
        glm::vec3 Albedo;               // Albedo
        glm::vec3 Ambient;              // Ambient
        glm::vec3 Diffuse;              // Diffuse
        glm::vec3 Specular;             // Specular
        glm::vec3 Emissive;             // Emissive
        float IOR = 1.5f;					              // Index of Refraction 0-128 (non-metallic vs metallic)
        float Shininess = 1.0f;                           // Shininess 0 = dull, 128 = shiny, sharp reflections
        float Opacity = 1.0f; // 1 opaque, 0 transparent
       
        /// <summary>Sets a texture of a specific type (e.g., diffuse, specular)</summary>
        /// <details>If texture type already exists, it replaces it.
        /// If texture type does NOT already exist, it adds a new texture.</details>
        /// <param name="map_type">NG_TEXTURE_TYPE_</param>
        /// <param name="texture">Texture</param>
        void SetTexture(const char* texture_type, Texture* texture);

		/// <summary>
		/// Finds a texture of a given TextureType
		/// </summary>
		/// <param name="type">TextureType</param>
		/// <returns>ptr* to found Texture object, or nullptr if not found</returns>
		Texture* FindTexture(TextureType type);

    };

}