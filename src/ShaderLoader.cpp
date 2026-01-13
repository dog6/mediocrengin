#include <AVGNG/ShaderLoader.hpp>
#include <AVGNG/Debug.hpp>

using namespace ng::Core;
using namespace ng::Graphics;

namespace ng::Assets {
    
    std::unordered_map<std::string, ng::Graphics::Shader*> ShaderLoader::shaderCache;

    // Helper Methods
    std::string ShaderLoader::ReadShaderFile(const char* shader_filePath)
    {

        std::ifstream file = FileReader::ReadFile(shader_filePath);
        if (!file.is_open()) {
            Debug::Log(LogLevel::ERROR, "Failed to load shader %s", shader_filePath);
            return "";
        }

        std::string vertShaderProgram;

        std::string line;
        while (std::getline(file, line)) {
            vertShaderProgram += line + "\n";
        }

        return vertShaderProgram;
    }

    Shader* ShaderLoader::LoadShaderFromFiles(const char* vertex_shader_filePath, const char* frag_shader_filePath)
    {

        Debug::Log(LogLevel::DEBUG, "Loading shaders:\n\t- Vertex: %s\n\t- Fragment: %s\n", vertex_shader_filePath, frag_shader_filePath);

        // Read vertex shader
        std::string vertShader = ReadShaderFile(vertex_shader_filePath);
        std::string fragShader = ReadShaderFile(frag_shader_filePath);

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

        return result;

    }

    /// <summary>
    /// Checks the shader cache for an existing shader with the given vertex and fragment shader paths.
    /// </summary>
    /// <param name="shaderName">Name of Shader</param>
    /// <returns>Shader* shader</returns>
    Shader* ShaderLoader::CheckShaderCacheForExistingShader(const char* shaderName) {

        if (shaderCache.find(shaderName) != shaderCache.end()) {
            return shaderCache[shaderName]; // Reuse existing shader
        }
        return nullptr;
    }

    /// <summary>
    /// Creates and loads a Shader object from a given vertex and fragment shader file path.
    /// </summary>
    /// <param name="vertShaderPath">Path to vertex shader file</param>
    /// <param name="fragShaderPath">Path to fragment shader file</param>
    /// <returns>Shader* shader</returns>
    Shader* ShaderLoader::LoadShader(const char* shaderName, const char* vertShaderPath, const char* fragShaderPath)
    {
        std::string key = shaderName;

        // Check cache first
        auto it = shaderCache.find(key);
        if (it != shaderCache.end()) {
            Debug::Log(LOG, "Shader cache hit: %s | ID: %d", key.c_str(), it->second->ID);
            return it->second;
        }

        Debug::Log(LOG, "Shader cache miss: %s. Loading new shader...", key.c_str());

        Shader* shader = LoadShaderFromFiles(vertShaderPath, fragShaderPath);

        if (!shader || shader->ID == 0) {
            Debug::Log(ERROR, "Failed to load shader from files:\n  Vertex: %s\n  Fragment: %s",
                vertShaderPath, fragShaderPath);
            if (shader) delete shader;
            return nullptr;
        }

        // Store in cache
        shaderCache[key] = shader;
        Debug::Log(LOG, "Shader loaded and cached successfully! ID: %d", shader->ID);

        return shader;
    }

    /// <summary>
    /// Creates and loads the default shader used for rendering.
    /// </summary>
    /// <returns>Shader* shader</returns>
    Shader* ShaderLoader::LoadDefaultShader() {

        Shader* shader;
        std::string fragShaderFolder = "res/shaders/fragShaders/";
        std::string vertShaderFolder = "res/shaders/vertexShaders/";

        std::string vertShaderPath = vertShaderFolder + "defaultShader.vert";
        std::string fragShaderPath = fragShaderFolder + "defaultShader.frag";

        Debug::Log(DEBUG, "Loading default vertex shader: %s", vertShaderPath.c_str());
        Debug::Log(DEBUG, "Loading default fragment shader: %s\n", fragShaderPath.c_str());

        shader = ShaderLoader::LoadShader("Default", vertShaderPath.c_str(), fragShaderPath.c_str());

        if (shader != nullptr) {
            Debug::Log(LOG, "Successfully loaded shader ID: %d", shader->ID);
            return shader;
        }

        return nullptr;

    }

}