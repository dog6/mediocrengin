#pragma once

#include <AVGNG/Mesh.hpp>
#include <AVGNG/Shader.hpp>
#include <AVGNG/Texture.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;
using namespace ng::Core;
using namespace ng::Graphics;

namespace ng::Assets {

    struct AssimpMesh {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<ng::Graphics::Texture> textures;
    };

    class AssimpObjLoader {
    public:
        static Texture* LoadTextureFromFile(const string& path);
        static AssimpMesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const string& directory);
        static void ProcessNode(aiNode* node, const aiScene* scene, vector<AssimpMesh>& meshes, const string& directory);
        static ng::Graphics::Mesh* LoadObjAsMesh(const string& path);
    };

}