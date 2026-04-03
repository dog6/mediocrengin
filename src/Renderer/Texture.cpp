#include "AVGNG/Renderer/Texture.hpp"
#include <string>

namespace ng::Graphics {

	Texture::Texture()
	{
		Texture::path = "";
		Texture::id = 0;
		Texture::hasDiffuseTexture = true; // has default diffuse texture by default
		Texture::hasSpecularTexture = false;
		Texture::hasAlphaTexture = false;
		Texture::hasEmissiveTexture = false;
		Texture::hasNormalTexture = false;

	}

	Texture::~Texture() {
		// Cleanup texture from GPU if needed
		if (id != 0) {
			glDeleteTextures(1, &id);
		}
	}

}