#include <glad/glad.h>
#include "AVGNG/Core/Debug.hpp"
#include "AVGNG/Graphics/ShaderLoader.hpp"
#include "AVGNG/Graphics/Shader.hpp"


using namespace std;
using namespace ng::Core;
using namespace ng::Graphics;

namespace ng::Assets {


    unordered_map<string, Shader> ShaderLoader::s_shaderCache = unordered_map<string, Shader>();


    // Helper Methods
    string ShaderLoader::ReadShaderFile(const char* shader_filePath)
    {
        string shaderProgram = FileReader::ReadFile(shader_filePath);
		if (shaderProgram.empty()) {
			Debug::Log(LogLevel::ERROR, "Shader file contents were empty: '%s'", shader_filePath);
			return "";
		}

        return shaderProgram;
    }

    Shader* ShaderLoader::LoadShaderFromFiles(const char* vertex_shader_filePath, const char* frag_shader_filePath)
    {

        Debug::Log(LogLevel::DEBUG, "Loading shaders:\n\t- Vertex: %s\n\t- Fragment: %s\n", vertex_shader_filePath, frag_shader_filePath);

        // Read vertex shader
        string vertShader = ReadShaderFile(vertex_shader_filePath);
        string fragShader = ReadShaderFile(frag_shader_filePath);

        Shader* result = new Shader();
        result->Build(vertShader.c_str(), fragShader.c_str());

        GLint success;
        glGetProgramiv(result->ID, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetProgramInfoLog(result->ID, 512, NULL, infoLog);
            Debug::Log(ERROR, "Shader linking failed for ID %d:\n%s", result->ID, infoLog);
            return nullptr;
        }

        Debug::Log(DEBUG, "Shader linked successfully! ID: %d", result->ID);

        result->vertex_shader_path = vertex_shader_filePath;
        result->fragment_shader_path = frag_shader_filePath;
        return result;

    }

    /// <summary>
    /// Checks the shader cache for an existing shader with the given vertex and fragment shader paths.
    /// </summary>
    /// <param name="shaderName">Name of Shader</param>
    /// <returns>Shader* shader</returns>
    Shader* ShaderLoader::CheckShaderCacheForExistingShader(const char* shaderName) {

        auto it = s_shaderCache.find(shaderName);
        return (it != s_shaderCache.end()) ? &it->second : nullptr;

        string key = string(shaderName);

		unordered_map<string, Shader>::iterator shaderIT = s_shaderCache.find(key);

        if (shaderIT == s_shaderCache.end()) {
			Debug::Log(DEBUG, "Shader '%s' not found in cache.", shaderName);
            return nullptr;
        }
		Debug::Log(DEBUG, "Shader '%s' found in cache.", shaderName);
        return &shaderIT->second; // return found cached shader
    
    }

    /// <summary>
    /// Creates and loads a Shader object from a given vertex and fragment shader file path.
    /// </summary>
    /// <param name="vertShaderPath">Path to vertex shader file</param>
    /// <param name="fragShaderPath">Path to fragment shader file</param>
    /// <returns>Shader* shader</returns>
    Shader ShaderLoader::LoadShader(const char* shaderName, const char* vertShaderPath, const char* fragShaderPath)
    {

        // Check cache first
		Shader* cachedShader = CheckShaderCacheForExistingShader(shaderName);
        if (cachedShader != nullptr) {
            Debug::Log(LOG, "Shader cache hit: %s | ID: %d", shaderName, cachedShader->ID);
            return *cachedShader;
        }      

        Debug::Log(LOG, "Shader cache miss: %s. Loading as new shader..", shaderName);
        
        Shader* shader = LoadShaderFromFiles(vertShaderPath, fragShaderPath);
        
        if (!shader || shader->ID == 0) {
            Debug::Log(ERROR, "Failed to load shader from files:\n  Vertex: %s\n  Fragment: %s", vertShaderPath, fragShaderPath);
			throw runtime_error("Failed to load shader from provided paths");
        }

        // Store in cache
        s_shaderCache[shaderName] = *shader;
        Debug::Log(LOG, "Shader loaded and cached successfully! ID: %d", shader->ID);

        return *shader;
    }

    /// <summary>
    /// Creates and loads the default shader used for rendering.
    /// </summary>
    /// <returns>Shader* shader</returns>
    Shader ShaderLoader::LoadDefaultShader() {


        Debug::Log(DEBUG, "Loading default vertex shader: %s", DEFAULT_VERTEX_SHADER_PATH);
        Debug::Log(DEBUG, "Loading default fragment shader: %s\n", DEFAULT_FRAGMENT_SHADER_PATH);

        Shader shader = ShaderLoader::LoadShader("Default", DEFAULT_VERTEX_SHADER_PATH, DEFAULT_FRAGMENT_SHADER_PATH);

        if (shader.ID == 0) {
			Debug::Log(ERROR, "Failed to load default shader.");
			throw runtime_error("Failed to load default shader.");
        }

        Debug::Log(LOG, "Successfully loaded shader ID: %d", shader.ID);
        return shader;

    }

}