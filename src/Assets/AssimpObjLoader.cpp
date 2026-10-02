#include <glad/glad.h>

#include "AVGNG/Assets/AssimpObjLoader.hpp"
#include "AVGNG/Assets/TextureLoader.hpp"
#include "AVGNG/Core/Debug.hpp"

#include "AVGNG/Graphics/Mesh.hpp"
#include "AVGNG/Graphics/Texture.hpp"
#include "AVGNG/Graphics/Material.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <filesystem>
#include <unordered_set>
#include <utility>

using namespace std;
using namespace ng::Core;
using namespace ng::Graphics;

namespace ng::Assets {

    namespace {

        string NormalizePath(const filesystem::path& rawPath)
        {
            return rawPath.lexically_normal().generic_string();
        }

        void SetDefaultColors(MaterialData* matData)
        {
            if (matData == nullptr) return;
            matData->Albedo  = glm::vec3(1.0f);
            matData->Diffuse = glm::vec3(1.0f);
            matData->Ambient = glm::vec3(1.0f);
        }

        void AppendVertices(const aiMesh* mesh, vector<Vertex>& vertices)
        {
            vertices.reserve(vertices.size() + mesh->mNumVertices);

            for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
                Vertex v;
                v.position = glm::vec3(mesh->mVertices[i].x,
                                       mesh->mVertices[i].y,
                                       mesh->mVertices[i].z);
                v.normal = mesh->HasNormals()
                    ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)
                    : glm::vec3(0.0f, 0.0f, 1.0f);
                v.texCoord = mesh->HasTextureCoords(0)
                    ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                    : glm::vec2(0.0f, 0.0f);

                vertices.push_back(v);
            }
        }

        void AppendIndices(const aiMesh* mesh, unsigned int indexOffset, vector<unsigned int>& indices)
        {
            for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
                const aiFace& face = mesh->mFaces[f];
                for (unsigned int i = 0; i < face.mNumIndices; ++i)
                    indices.push_back(face.mIndices[i] + indexOffset);
            }
        }

        bool TryLoadTexture(const aiMaterial* material,
                            aiTextureType assimpType,
                            TextureType engineType,
                            const filesystem::path& objDir,
                            MaterialData& matData)
        {
            if (material->GetTextureCount(assimpType) == 0) return false;

            aiString texPath;
            if (material->GetTexture(assimpType, 0, &texPath) != AI_SUCCESS) return false;
            if (texPath.C_Str()[0] == '*') return false; // Skip embedded textures

            string relPath = texPath.C_Str();
            if (!relPath.empty() && (relPath[0] == '/' || relPath[0] == '\\'))
                relPath = relPath.substr(1);

            const filesystem::path fullPath = (objDir / relPath).lexically_normal();

            Texture* texture = TextureLoader::LoadFromFile(fullPath.string());
            if (texture == nullptr) return false;

            // MaterialData owns the texture after this call.
            // SetTexture also sets texture->type.
            matData.SetTexture(engineType, texture);

            Debug::Log(LogLevel::DEBUG, "[AssimpLoader] Set texture '%s' (%s)",
                       TextureLoader::GetTextureKey(engineType), fullPath.string().c_str());
            return true;
        }

        void LoadMaterialTextures(const aiMaterial* material,
                                  const filesystem::path& objDir,
                                  MaterialData& matData)
        {
            const aiTextureType diffuseTypes[] = {
                aiTextureType_DIFFUSE,
                aiTextureType_BASE_COLOR,
                aiTextureType_UNKNOWN
            };
            for (aiTextureType type : diffuseTypes) {
                if (TryLoadTexture(material, type, TextureType::DIFFUSE, objDir, matData))
                    break;
            }

            struct MapEntry { aiTextureType assimp; TextureType engine; };
            const MapEntry secondaryMaps[] = {
                { aiTextureType_SPECULAR,          TextureType::SPECULAR },
                { aiTextureType_HEIGHT,            TextureType::NORMAL   },
                { aiTextureType_NORMALS,           TextureType::NORMAL   },
                { aiTextureType_EMISSIVE,          TextureType::EMISSIVE },
                { aiTextureType_OPACITY,           TextureType::ALPHA    },
                { aiTextureType_DIFFUSE_ROUGHNESS, TextureType::METALLIC }
            };
            for (const MapEntry& entry : secondaryMaps)
                TryLoadTexture(material, entry.assimp, entry.engine, objDir, matData);
        }

    } // anonymous namespace

    Mesh* AssimpObjLoader::LoadObjAsMesh(const string& rawPath)
    {
        string path = NormalizePath(rawPath);
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_GenNormals |
            aiProcess_CalcTangentSpace);

        if (!scene || !scene->HasMeshes()) {
            Debug::Log(LogLevel::ERROR, "Failed to load OBJ: %s", path.c_str());
            return nullptr;
        }

        const filesystem::path objDir = filesystem::path(path).parent_path();

        vector<Vertex> vertices;
        vector<unsigned int> indices;
        unsigned int indexOffset = 0;

        Material* meshMaterial = new Material();
        MaterialData* matData = meshMaterial->GetMaterialData();
        SetDefaultColors(matData);

        unordered_set<unsigned int> loadedMaterials;

        for (unsigned int m = 0; m < scene->mNumMeshes; ++m) {
            const aiMesh* mesh = scene->mMeshes[m];

            AppendVertices(mesh, vertices);
            AppendIndices(mesh, indexOffset, indices);
            indexOffset += mesh->mNumVertices;

            const unsigned int matIndex = mesh->mMaterialIndex;
            const bool canLoadMaterial =
                scene->HasMaterials() &&
                matData != nullptr &&
                matIndex < scene->mNumMaterials &&
                loadedMaterials.insert(matIndex).second;

            if (canLoadMaterial) {
                const aiMaterial* aiMat = scene->mMaterials[matIndex];
                if (aiMat != nullptr)
                    LoadMaterialTextures(aiMat, objDir, *matData);
            }
        }

        Mesh* outMesh = new Mesh();
        outMesh->vertices = std::move(vertices);
        outMesh->indices  = std::move(indices);
        outMesh->filepath = path;
        outMesh->material = meshMaterial;

        outMesh->SetupMesh();

        return outMesh;
    }

}