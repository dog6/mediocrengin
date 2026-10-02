#pragma once

#include "AVGNG/Graphics/Texture.hpp"
#include <array>
#include <cstddef>
#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace ng::Graphics {

    // Tiling and offset for one texture slot. The default is 1:1.
    struct TextureTransform
    {
        glm::vec2 tiling{ 1.0f, 1.0f };
        glm::vec2 offset{ 0.0f, 0.0f };
    };

    class MaterialData
    {
        bool hasDiffuseTexture = false;
        bool hasSpecularTexture = false;
        bool hasNormalTexture = false;
        bool hasEmissiveTexture = false;
        bool hasAlphaTexture = false;
        bool hasMetallicTexture = false;

        std::vector<ng::Graphics::Texture*> textures;        // One slot for each TextureType
        std::vector<ng::Graphics::Texture*> retiredTextures; // Waiting to be freed
        std::array<TextureTransform, 6> textureTransforms;   // One entry for each slot

    public:
        static constexpr std::size_t TEXTURE_SLOT_COUNT = 6;

        MaterialData();
        ~MaterialData();

        // MaterialData owns the texture pointers. A copy would free them two times.
        MaterialData(const MaterialData&) = delete;
        MaterialData& operator=(const MaterialData&) = delete;

        MaterialData(MaterialData&& other) noexcept;
        MaterialData& operator=(MaterialData&& other) noexcept;

        std::string name;

        // Color values
        glm::vec3 Albedo;
        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Specular;
        glm::vec3 Emissive;

        // Properties
        float IOR = 1.5f;
        float Shininess = 1.0f;
        float Opacity = 1.0f;
        float Metallicness = 0.0f;

        /// MaterialData owns the texture after this call.
        /// The old texture in the slot is freed later (see ReleaseRetiredTextures).
        void SetTexture(const char* texture_type, Texture* texture);
        void SetTexture(TextureType type, Texture* texture);

        /// Returns nullptr if the slot is empty.
        Texture* FindTexture(TextureType type);

        /// Removes the texture from a slot. The texture is freed later.
        void RemoveTexture(TextureType type);

        /// Frees replaced or removed textures. Call one time each frame.
        void ReleaseRetiredTextures();

        /// Returns the tiling and offset of a slot. The default is 1:1.
        TextureTransform& GetTextureTransform(TextureType type);
    };

}