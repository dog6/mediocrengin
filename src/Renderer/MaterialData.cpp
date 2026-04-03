#include "AVGNG/Renderer/MaterialData.hpp"
#include "AVGNG/Utilities/Debug.hpp"


using namespace ng::Core; // for debug

namespace ng::Graphics {

	std::vector<ng::Graphics::Texture> textures;

	TextureType GetTextureTypeFromCStr(const char* texture_type) {
		if (strcmp(texture_type, TEXTURE_TYPE_DIFFUSE) == 0) {
			return TextureType::DIFFUSE;
		}
		else if (strcmp(texture_type, TEXTURE_TYPE_SPECULAR) == 0) {
			return TextureType::SPECULAR;
		}
		else if (strcmp(texture_type, TEXTURE_TYPE_NORMAL) == 0) {
			return TextureType::NORMAL;
		}
		else if (strcmp(texture_type, TEXTURE_TYPE_EMISSIVE) == 0) {
			return TextureType::EMISSIVE;
		}
		else if (strcmp(texture_type, TEXTURE_TYPE_ALPHA) == 0) {
			return TextureType::ALPHA;
		}
		else if (strcmp(texture_type, TEXTURE_TYPE_METALLIC) == 0) {
			return TextureType::METALLIC;
		}
		else {
			Debug::Log(WARN, "Unknown texture type string: '%s'", texture_type);
			return TextureType::DIFFUSE; // default
		}
	}

	MaterialData::MaterialData() {
		// Default constructor
		MaterialData::Albedo = glm::vec3(1.0f, 0.0f, 1.0f);
		MaterialData::Ambient = glm::vec3(1.0f);
		MaterialData::Diffuse = glm::vec3(1.0f);
		MaterialData::Specular = glm::vec3(0.0f);
		MaterialData::Emissive = glm::vec3(0.0f);
		textures.resize(6);

	}

	MaterialData::~MaterialData()
	{
		if (textures.size() > 0) {
			textures.clear();
		}
	}

	void MaterialData::SetTexture(const char* texture_type, Texture* texture)
	{
		if (!texture_type) {
			Debug::Log(ERROR, "SetTexture called with null texture_type pointer!");
			return;
		}

		if (texture->id == 0) {
			Debug::Log(WARN, "SetTexture called with texture ID 0 for type '%s'!", texture_type);
		}


		// If not found, add new texture
		texture->type = std::string(texture_type);
		textures[(int)GetTextureTypeFromCStr(texture_type)] = texture;
	}


	Texture* MaterialData::FindTexture(TextureType type) {
		if (type < 0 || type >= textures.size()) {
			return nullptr;
		}
		else {
			return textures[type];
		}
	}

}
