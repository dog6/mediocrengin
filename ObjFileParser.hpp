// objFileParser.hpp
#pragma once

#ifndef FILE_PARSER_OBJ_HPP
#define FILE_PARSER_OBJ_HPP

#include <string>
#include <vector>
#include <string_view>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <filesystem>

#include "TexCoord.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "FileReader.hpp"





using namespace std;
using namespace ng::Core;
using namespace ng::Assets;
using namespace ng::Graphics;

namespace ng {
namespace Assets {

        class ObjFileParser {
        public:
            static ng::Graphics::Mesh* LoadObjFromFile(const std::string& objFilePath);
            static std::unordered_map<std::string, Material> LoadMaterialFromFile(const std::string& mtlPath);
        };

    } 
}

#endif