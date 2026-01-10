#include <AVGNG/ShaderLoader.hpp>

using namespace ng::Core;
using namespace ng::Graphics;

namespace ng::Assets {
    
    Shader* ShaderLoader::LoadShader(const char* vertShaderPath, const char* fragShaderPath)
    {
        return Shader::LoadShader(vertShaderPath, fragShaderPath);
    }
    
    
    Shader* ShaderLoader::LoadDefaultShader() {
        Shader* shader;
        std::string fragShaderFolder = "res/shaders/fragShaders/";
        std::string vertShaderFolder = "res/shaders/vertexShaders/";

        std::string vertShaderPath = vertShaderFolder + "defaultShader.vert";
        std::string fragShaderPath = fragShaderFolder + "defaultShader.frag";

        Debug::Log(DEBUG, "Loading default vertex shader: %s", vertShaderPath.c_str());
        Debug::Log(DEBUG, "Loading default fragment shader: %s\n", fragShaderPath.c_str());

        shader = ng::Graphics::Shader::LoadShader(vertShaderPath.c_str(), fragShaderPath.c_str());

        if (shader != nullptr) {
            Debug::Log(LOG, "Successfully loaded shader ID: %d", shader->ID);
            return shader;
        }
        return nullptr;
    }



}