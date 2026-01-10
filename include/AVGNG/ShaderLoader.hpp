#pragma once


#include <AVGNG/Shader.hpp>
#include <AVGNG/Debug.hpp>

namespace ng::Assets {

	class ShaderLoader {

	public:
		static ng::Graphics::Shader* LoadShader(const char* vertShaderPath, const char* fragShaderPath);
		static ng::Graphics::Shader* LoadDefaultShader();

	};

}