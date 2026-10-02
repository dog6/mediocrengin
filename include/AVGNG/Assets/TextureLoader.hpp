#pragma once
#include <string>
#include "AVGNG/Graphics/Texture.hpp"

namespace ng::Assets {

class TextureLoader {
public:
    TextureLoader() = delete;

    static ng::Graphics::Texture* LoadFromFile(const std::string& rawPath);
    static const char* GetTextureKey(ng::Graphics::TextureType type);
    static void Destroy(ng::Graphics::Texture* texture);
};

}