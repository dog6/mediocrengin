#pragma once

#define NG_QUIET_PARSING

#include <string>
#include <vector>
#include <string_view>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <filesystem>

#include <AVGNG/TexCoord.hpp>
#include <AVGNG/Mesh.hpp>
#include <AVGNG/Material.hpp>
#include <AVGNG/FileReader.hpp>

#include <AVGNG/ShaderLoader.hpp>
#include <AVGNG/GameObject.hpp>
#include <AVGNG/Utility.hpp>

namespace ng::Assets {


    class ObjFileParser {

      
        public:
            static ng::Core::GameObject* LoadObjAsGameObject(const char* name, const char* objFilePath, ng::Graphics::Shader* shader);
            static ng::Graphics::Mesh* LoadObjFromFileAsMesh(const std::string& objFilePath);
            static std::unordered_map<std::string, ng::Graphics::MaterialData> LoadMaterialsFromFile(const std::string& mtlPath);
        };

} 

