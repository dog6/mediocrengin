#pragma once
#include <string>

namespace ng::Graphics { class Mesh; }

namespace ng::Assets {

    class AssimpObjLoader {
    public:
        static ng::Graphics::Mesh* LoadObjAsMesh(const std::string& rawPath);
    };

}