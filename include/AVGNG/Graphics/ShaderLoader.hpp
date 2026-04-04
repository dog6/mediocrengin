#pragma once

#include "AVGNG/Graphics/Shader.hpp"
#include <string>
// #include <xstring>
#include <unordered_map>

namespace ng::Assets {

	class ShaderLoader {

	private:
		static std::unordered_map<std::string, ng::Graphics::Shader> s_shaderCache;
		static ng::Graphics::Shader* CheckShaderCacheForExistingShader(const char* shaderName);
		static std::string ReadShaderFile(const char* shader_filePath);
		static ng::Graphics::Shader* LoadShaderFromFiles(const char* vertex_shader_filePath, const char* frag_shader_filePath);
	public:
		static ng::Graphics::Shader LoadShader(const char* shaderName, const char* vertShaderPath, const char* fragShaderPath);
		static ng::Graphics::Shader LoadDefaultShader();

	};

}