// objFileParser.hpp
#pragma once

#ifndef FILE_PARSER_OBJ_HPP
#define FILE_PARSER_OBJ_HPP

#include <string>
#include <vector>
#include <string_view>
#include <fstream>
#include <sstream>
#include "Mesh.hpp"

namespace ng {
    namespace Assets {

        class ObjFileParser {
        public:
            static ng::Graphics::Mesh* LoadObjFromFile(const std::string& objFilePath);
        };

    } 
}

#endif