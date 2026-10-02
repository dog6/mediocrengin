#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "AVGNG/Graphics/MaterialData.hpp"
#include "AVGNG/Core/Debug.hpp"

#include <cstring>
#include <string>
#include <utility>

using namespace ng::Core; // for debug

namespace ng::Graphics {

	namespace {

		TextureType GetTextureTypeFromCStr(const char* texture_type) {
			if (strcmp(texture_type, TEXTURE_TYPE_DIFFUSE) == 0)       return TextureType::DIFFUSE;
			else if (strcmp(texture_type, TEXTURE_TYPE_SPECULAR) == 0) return TextureType::SPECULAR;
			else if (strcmp(texture_type, TEXTURE_TYPE_NORMAL) == 0)   return TextureType::NORMAL;
			else if (strcmp(texture_type, TEXTURE_TYPE_EMISSIVE) == 0) return TextureType::EMISSIVE;
			else if (strcmp(texture_type, TEXTURE_TYPE_ALPHA) == 0)    return TextureType::ALPHA;
			else if (strcmp(texture_type, TEXTURE_TYPE_METALLIC) == 0) return TextureType::METALLIC;

			Debug::Log(WARN, "Unknown texture type string: '%s'", texture_type);
			return TextureType::DIFFUSE;
		}

		const char* GetCStrFromTextureType(TextureType type) {
			switch (type) {
				case TextureType::DIFFUSE:  return TEXTURE_TYPE_DIFFUSE;
				case TextureType::SPECULAR: return TEXTURE_TYPE_SPECULAR;
				case TextureType::NORMAL:   return TEXTURE_TYPE_NORMAL;
				case TextureType::EMISSIVE: return TEXTURE_TYPE_EMISSIVE;
				case TextureType::ALPHA:    return TEXTURE_TYPE_ALPHA;
				case TextureType::METALLIC: return TEXTURE_TYPE_METALLIC;
				default:                    return TEXTURE_TYPE_DIFFUSE;
			}
		}

		bool IsValidSlot(TextureType type) {
			const int index = (int)type;
			return index >= 0 && index < (int)MaterialData::TEXTURE_SLOT_COUNT;
		}

		// Frees the OpenGL texture and the Texture object.
		void DestroyTexture(Texture* texture) {
			if (texture == nullptr) return;

			// At shutdown the OpenGL context can be gone already.
			if (texture->id != 0 && glfwGetCurrentContext() != nullptr)
				glDeleteTextures(1, &texture->id);

			delete texture;
		}

	} // anonymous namespace

	MaterialData::MaterialData() {
		Albedo   = glm::vec3(1.0f, 0.0f, 1.0f);
		Ambient  = glm::vec3(1.0f);
		Diffuse  = glm::vec3(1.0f);
		Specular = glm::vec3(0.0f);
		Emissive = glm::vec3(0.0f);

		textures.assign(TEXTURE_SLOT_COUNT, nullptr);
	}

	MaterialData::~MaterialData()
	{
		for (Texture* texture : textures)
			DestroyTexture(texture);
		textures.clear();

		ReleaseRetiredTextures();
	}

	MaterialData::MaterialData(MaterialData&& other) noexcept
	{
		textures.assign(TEXTURE_SLOT_COUNT, nullptr);
		*this = std::move(other);
	}

	MaterialData& MaterialData::operator=(MaterialData&& other) noexcept
	{
		if (this == &other) return *this;

		// Free the textures that this object owns now.
		for (Texture* texture : textures)
			DestroyTexture(texture);
		ReleaseRetiredTextures();

		name         = std::move(other.name);
		Albedo       = other.Albedo;
		Ambient      = other.Ambient;
		Diffuse      = other.Diffuse;
		Specular     = other.Specular;
		Emissive     = other.Emissive;
		IOR          = other.IOR;
		Shininess    = other.Shininess;
		Opacity      = other.Opacity;
		Metallicness = other.Metallicness;

		hasDiffuseTexture  = other.hasDiffuseTexture;
		hasSpecularTexture = other.hasSpecularTexture;
		hasNormalTexture   = other.hasNormalTexture;
		hasEmissiveTexture = other.hasEmissiveTexture;
		hasAlphaTexture    = other.hasAlphaTexture;
		hasMetallicTexture = other.hasMetallicTexture;

		// Take the textures. The other object must not free them.
		textures          = std::move(other.textures);
		retiredTextures   = std::move(other.retiredTextures);
		textureTransforms = other.textureTransforms;

		other.textures.assign(TEXTURE_SLOT_COUNT, nullptr);
		other.retiredTextures.clear();

		return *this;
	}

	void MaterialData::SetTexture(const char* texture_type, Texture* texture)
	{
		if (!texture_type) {
			Debug::Log(ERROR, "SetTexture called with null texture_type pointer!");
			return;
		}

		SetTexture(GetTextureTypeFromCStr(texture_type), texture);
	}

	void MaterialData::SetTexture(TextureType type, Texture* texture)
	{
		if (texture == nullptr) {
			Debug::Log(ERROR, "SetTexture called with a null texture!");
			return;
		}

		if (!IsValidSlot(type)) {
			Debug::Log(ERROR, "SetTexture called with an invalid texture type: %d", (int)type);
			return;
		}

		if (texture->id == 0) {
			Debug::Log(WARN, "SetTexture called with texture ID 0 for type '%s'!",
			           GetCStrFromTextureType(type));
		}

		Texture*& slot = textures[(size_t)type];

		if (slot == texture) return;

		// Do not free the old texture now. ImGui can still draw it in this frame.
		if (slot != nullptr)
			retiredTextures.push_back(slot);

		texture->type = std::string(GetCStrFromTextureType(type));
		slot = texture;
	}

	Texture* MaterialData::FindTexture(TextureType type)
	{
		if (!IsValidSlot(type)) return nullptr;
		return textures[(size_t)type];
	}

	void MaterialData::RemoveTexture(TextureType type)
	{
		if (!IsValidSlot(type)) return;

		Texture*& slot = textures[(size_t)type];
		if (slot == nullptr) return;

		retiredTextures.push_back(slot);
		slot = nullptr;
	}

	void MaterialData::ReleaseRetiredTextures()
	{
		for (Texture* texture : retiredTextures)
			DestroyTexture(texture);
		retiredTextures.clear();
	}

	TextureTransform& MaterialData::GetTextureTransform(TextureType type)
	{
		if (!IsValidSlot(type)) {
			Debug::Log(ERROR, "GetTextureTransform called with an invalid texture type: %d", (int)type);
			return textureTransforms[0];
		}
		return textureTransforms[(size_t)type];
	}

}