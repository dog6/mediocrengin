#pragma once

#include <string>

// 🔌 Clean forward declaration for pointer targets
namespace ng::Graphics {
    class Mesh;
}

namespace ng::Assets {

    class AssimpObjLoader {
    public:
        // Public API strictly uses string references and pointers
        static ng::Graphics::Mesh* LoadObjAsMesh(const std::string& path);
    };

}