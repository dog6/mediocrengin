// objFileParser.hpp
#pragma once

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

namespace ng::Assets {

        class ObjFileParser {
        public:
            static ng::Graphics::Mesh* LoadObjFromFile(const std::string& objFilePath);
            static std::unordered_map<std::string, ng::Graphics::Material> LoadMaterialFromFile(const std::string& mtlPath);
        };

    } 

