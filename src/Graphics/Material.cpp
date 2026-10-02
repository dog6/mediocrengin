#include <glad/glad.h>

#include "AVGNG/Graphics/Material.hpp"
#include "AVGNG/Core/Debug.hpp"

#include <utility>

using namespace ng::Core; // for debug

namespace ng::Graphics {

	namespace {
		bool s_defaultShaderLoaded = false;
	}

	ng::Graphics::Shader Material::s_defaultShader;

	Material::Material()
	{
		if (!s_defaultShaderLoaded) {
			s_defaultShader = ng::Assets::ShaderLoader::LoadDefaultShader();
			s_defaultShaderLoaded = true;
		}
		shader = s_defaultShader;
	}

	Material::~Material() {}

	void Material::SetMaterialData(MaterialData&& materialData)
	{
		Debug::Log(DEBUG, "Setting material data");
		data = std::move(materialData);
	}

	Shader* Material::GetShader()
	{
		return &shader;
	}

	void Material::SetAlbedoColor(glm::vec3 col)   { this->data.Albedo = col; }
	void Material::SetAmbientColor(glm::vec3 col)  { this->data.Ambient = col; }
	void Material::SetDiffuseColor(glm::vec3 col)  { this->data.Diffuse = col; }
	void Material::SetSpecularColor(glm::vec3 col) { this->data.Specular = col; }
	void Material::SetEmissiveColor(glm::vec3 col) { this->data.Emissive = col; }

}