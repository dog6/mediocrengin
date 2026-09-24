#pragma once

#include "AVGNG/Graphics/Shader.hpp"
#include <string>
// #include <xstring>
#include <unordered_map>

        // string vertShaderPath = ;
        // string fragShaderPath = ;

#define DEFAULT_VERTEX_SHADER_PATH "D:/Projects/CPP/smallengine/res/shaders/vertexShaders/devShader.vert"
#define DEFAULT_FRAGMENT_SHADER_PATH "D:/Projects/CPP/smallengine/res/shaders/fragShaders/devShader.frag"

namespace ng::Assets {

	/// @brief Loads shader files into memory stored in s_shaderCache for static access.
	class ShaderLoader {

		private:
			static std::unordered_map<std::string, ng::Graphics::Shader> s_shaderCache; // shader cache
			static ng::Graphics::Shader* CheckShaderCacheForExistingShader(const char* shaderName); // get shader from cache
			static std::string ReadShaderFile(const char* shader_filePath); // read shader file contents
			static ng::Graphics::Shader* LoadShaderFromFiles(const char* vertex_shader_filePath, const char* frag_shader_filePath); // read shader using vert & frag shader file paths
		public:
			static ng::Graphics::Shader LoadShader(const char* shaderName, const char* vertShaderPath, const char* fragShaderPath); // load shader into cache
			static ng::Graphics::Shader LoadDefaultShader(); // loads default shader using default path defined in header file

	};

}