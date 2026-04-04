#pragma once

#include <GLFW/glfw3.h>
#include <string>

namespace ng::Graphics {


    constexpr const char* TEXTURE_TYPE_DIFFUSE = "diffuseMap";
    constexpr const char* TEXTURE_TYPE_SPECULAR = "specularMap";
    constexpr const char* TEXTURE_TYPE_NORMAL = "normalMap";
    constexpr const char* TEXTURE_TYPE_EMISSIVE = "emissiveMap";
    constexpr const char* TEXTURE_TYPE_ALPHA = "alphaMap";
    constexpr const char* TEXTURE_TYPE_METALLIC = "metallicMap";

	enum TextureType {
		DIFFUSE = 0,
		SPECULAR,
		NORMAL,
		EMISSIVE,
		ALPHA,
        METALLIC
	};

    class Texture {

    public:
        Texture();
        ~Texture();
        GLuint id = 0;
        std::string type;
        std::string path;
        int height, width;
        GLenum internalFormat;
        GLenum dataFormat;
        unsigned char* data;
        bool hasDiffuseTexture = type == TEXTURE_TYPE_DIFFUSE && id > 0;
        bool hasSpecularTexture = type == TEXTURE_TYPE_SPECULAR && id > 0;
        bool hasNormalTexture = type == TEXTURE_TYPE_NORMAL && id > 0;
        bool hasEmissiveTexture = type == TEXTURE_TYPE_EMISSIVE && id > 0;
        bool hasAlphaTexture = type == TEXTURE_TYPE_ALPHA && id > 0;
        bool hasMetallicTexture = type == TEXTURE_TYPE_METALLIC && id > 0;

    };

}