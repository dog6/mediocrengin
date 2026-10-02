#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "AVGNG/Assets/TextureLoader.hpp"
#include "AVGNG/Core/Debug.hpp"

#include <filesystem>

using namespace ng::Core;
using namespace ng::Graphics;

namespace ng::Assets {

    Texture* TextureLoader::LoadFromFile(const std::string& rawPath)
    {
        const std::string path =
            std::filesystem::path(rawPath).lexically_normal().generic_string();

        Texture* tex = new Texture();

        int nrChannels = 0;
        tex->data = stbi_load(path.c_str(), &tex->width, &tex->height, &nrChannels, 0);
        if (!tex->data) {
            Debug::Log(LogLevel::ERROR, "[TextureLoader] Failed to load texture: %s", path.c_str());
            delete tex;
            return nullptr;
        }

        if (nrChannels == 1)      { tex->internalFormat = GL_R8;    tex->dataFormat = GL_RED;  }
        else if (nrChannels == 3) { tex->internalFormat = GL_RGB8;  tex->dataFormat = GL_RGB;  }
        else if (nrChannels == 4) { tex->internalFormat = GL_RGBA8; tex->dataFormat = GL_RGBA; }
        else {
            Debug::Log(LogLevel::ERROR, "[TextureLoader] Unsupported channel count: %d in %s",
                       nrChannels, path.c_str());
            stbi_image_free(tex->data);
            delete tex;
            return nullptr;
        }

        glGenTextures(1, &tex->id);
        glBindTexture(GL_TEXTURE_2D, tex->id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, tex->internalFormat, tex->width, tex->height,
                     0, tex->dataFormat, GL_UNSIGNED_BYTE, tex->data);

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(tex->data);
        tex->data = nullptr;

        Debug::Log(LogLevel::DEBUG, "[TextureLoader] Loaded texture %s -> ID %u",
                   path.c_str(), tex->id);
        return tex;
    }

    const char* TextureLoader::GetTextureKey(TextureType type)
    {
        switch (type) {
            case TextureType::DIFFUSE:  return "diffuseMap";
            case TextureType::SPECULAR: return "specularMap";
            case TextureType::NORMAL:   return "normalMap";
            case TextureType::EMISSIVE: return "emissiveMap";
            case TextureType::ALPHA:    return "alphaMap";
            case TextureType::METALLIC: return "metallicMap";
            default:                    return "diffuseMap";
        }
    }

    void TextureLoader::Destroy(Texture* texture)
    {
        if (texture == nullptr) return;

        if (texture->id != 0)
            glDeleteTextures(1, &texture->id);

        delete texture;
    }

}