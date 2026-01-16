#include <AVGNG/MaterialData.hpp>
#include <AVGNG/Debug.hpp>


using namespace ng::Core; // for debug

namespace ng::Graphics {

	std::vector<ng::Graphics::Texture> textures(5);

	MaterialData::MaterialData() {
		// Default constructor
		MaterialData::Albedo = glm::vec3(1.0f, 0.0f, 1.0f);
		MaterialData::Ambient = glm::vec3(1.0f);
		MaterialData::Diffuse = glm::vec3(1.0f);
		MaterialData::Specular = glm::vec3(0.0f);
		MaterialData::Emissive = glm::vec3(0.0f);
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

		if (textures.size() > 0) {
			// Check if texture of this type already exists
			for (auto& tex : textures) {
				if (tex->type.c_str() == texture_type) {
					tex = texture; // Replace existing texture
					return;
				}
			}
		}

		// If not found, add new texture
		texture->type = std::string(texture_type);
		textures.push_back(texture);
	}


	Texture* MaterialData::FindTexture(const char* texture_type) {

		if (!texture_type || texture_type[0] == '\0') {
			Debug::Log(ERROR, "FindTexture called with empty texture_type!");
			return nullptr;
		}

		if (textures.empty()) {
			Debug::Log(WARN, "No textures available when searching for: '%s'", texture_type);
			return nullptr;
		}

		std::string typeStr(texture_type);

		for (auto& tex : textures) {
			if (tex->type == typeStr) {
				return tex;
			}
		}

		Debug::Log(WARN, "Failed to find texture type: '%s'", texture_type);
		return nullptr;
	}

}
