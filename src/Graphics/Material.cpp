#include "AVGNG/Graphics/Material.hpp"
#include "AVGNG/Graphics/ShaderLoader.hpp"
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

}
