#include <glad/glad.h> 

#include "AVGNG/Graphics/Material.hpp"
#include "AVGNG/Core/Debug.hpp"
// Default material constructor

using namespace ng::Core; // for debug

namespace ng::Graphics {


	ng::Graphics::Shader Material::s_defaultShader;

	Material::Material()
	{
		data = MaterialData();
		s_defaultShader = ng::Assets::ShaderLoader::LoadDefaultShader();
		shader = s_defaultShader;
	}

	Material::~Material() {}

	void Material::SetMaterialData(MaterialData materialData)
	{
		Debug::Log(DEBUG, "Setting material data");
		data = materialData;
	}

	Shader* Material::GetShader()
	{

		if (!&shader && &s_defaultShader) {
			Debug::Log(ERROR, "Assigned default shader to material missing shader");
			shader = s_defaultShader;
		}

		return &shader;
	}

    void Material::SetAlbedoColor(glm::vec3 col)
    {
		this->data.Albedo = col;
    }

    void Material::SetAmbientColor(glm::vec3 col)
    {
		this->data.Ambient = col;
    }

    void Material::SetDiffuseColor(glm::vec3 col)
    {
		this->data.Diffuse = col;
    }

    void Material::SetSpecularColor(glm::vec3 col)
    {
		this->data.Specular = col;
    }
	
    void Material::SetEmissiveColor(glm::vec3 col)
    {
		this->data.Emissive = col;
    }

}
