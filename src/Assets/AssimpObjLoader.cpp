#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "AVGNG/Assets/AssimpObjLoader.hpp"
#include "AVGNG/Core/Debug.hpp"

#include "AVGNG/Graphics/Mesh.hpp"
#include "AVGNG/Graphics/Shader.hpp"
#include "AVGNG/Graphics/Texture.hpp"
#include "AVGNG/Graphics/Material.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>

using namespace std;
using namespace ng::Core;
using namespace ng::Graphics;

namespace ng::Assets {

    static string NormalizePath(const filesystem::path& rawPath) {
        return rawPath.lexically_normal().generic_string();
    }

    static Texture* LoadTextureFromFile(const string& rawPath) {
        string path = NormalizePath(rawPath);
        Texture* tex = new Texture();

        int nrChannels;
        tex->data = stbi_load(path.c_str(), &tex->width, &tex->height, &nrChannels, 0);
        if (!tex->data) {
            Debug::Log(LogLevel::ERROR, "[LoadTextureFromFile] Failed to load texture: %s", path.c_str());
            delete tex;
            return nullptr;
        }

        if (nrChannels == 1)      { tex->internalFormat = GL_R8;   tex->dataFormat = GL_RED;  }
        else if (nrChannels == 3) { tex->internalFormat = GL_RGB8;  tex->dataFormat = GL_RGB;  }
        else if (nrChannels == 4) { tex->internalFormat = GL_RGBA8; tex->dataFormat = GL_RGBA; }
        else {
            Debug::Log(LogLevel::ERROR, "Unsupported texture channels: %d in %s", nrChannels, path.c_str());
            stbi_image_free(tex->data);
            delete tex;
            return nullptr;
        }

        glGenTextures(1, &tex->id);
        glBindTexture(GL_TEXTURE_2D, tex->id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, tex->internalFormat, tex->width, tex->height,
            0, tex->dataFormat, GL_UNSIGNED_BYTE, tex->data);

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(tex->data);
        tex->data = nullptr;

        Debug::Log(LogLevel::DEBUG, "Successfully loaded texture %s -> ID %u", path.c_str(), tex->id);
        return tex;
    }

    Mesh* AssimpObjLoader::LoadObjAsMesh(const string& rawPath) {
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

        vector<Vertex> vertices;
        vector<unsigned int> indices;
        filesystem::path objDir = filesystem::path(path).parent_path();
        unsigned int indexOffset = 0;

        // Instantiate material and set default base colors to white (1, 1, 1)
        Material* meshMaterial = new Material();
        MaterialData* matData = meshMaterial->GetMaterialData();
        if (matData != nullptr) {
            matData->Albedo  = glm::vec3(1.0f);
            matData->Diffuse = glm::vec3(1.0f);
            matData->Ambient = glm::vec3(1.0f);
        }

        for (unsigned int m = 0; m < scene->mNumMeshes; ++m) {
            aiMesh* mesh = scene->mMeshes[m];

            // 1. Load Vertices
            for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
                Vertex v;
                v.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
                v.normal   = mesh->HasNormals() ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)
                                                : glm::vec3(0.0f, 0.0f, 1.0f);
                v.texCoord = mesh->HasTextureCoords(0)
                    ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                    : glm::vec2(0.0f, 0.0f);
                
                vertices.push_back(v);
            }

            // 2. Load Indices
            for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
                aiFace face = mesh->mFaces[f];
                for (unsigned int idx = 0; idx < face.mNumIndices; ++idx)
                    indices.push_back(face.mIndices[idx] + indexOffset);
            }
            indexOffset += mesh->mNumVertices;

            // 3. Load Materials & Textures
          // 3. Load Materials & Textures
if (scene->HasMaterials() && mesh->mMaterialIndex >= 0) {
    aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];
    if (ai_material != nullptr && matData != nullptr) {

       auto loadAndAttachTexture = [&](aiTextureType assimpType, TextureType engineType) -> bool {
            if (ai_material->GetTextureCount(assimpType) == 0) return false;

            aiString texPath;
            if (ai_material->GetTexture(assimpType, 0, &texPath) != AI_SUCCESS) return false;
            if (texPath.C_Str()[0] == '*') return false; // Skip embedded textures

            string relPath = texPath.C_Str();
            if (!relPath.empty() && (relPath[0] == '/' || relPath[0] == '\\')) {
                relPath = relPath.substr(1);
            }

            filesystem::path fullPath = (objDir / relPath).lexically_normal();
            Texture* texture = LoadTextureFromFile(fullPath.string());

            if (texture != nullptr) {
                texture->type = engineType;
                
                // Map engine enum to shader uniform string key expected by SetTexture
                const char* textureKey = "diffuseMap";
                switch (engineType) {
                    case TextureType::DIFFUSE:  textureKey = "diffuseMap";  break;
                    case TextureType::SPECULAR: textureKey = "specularMap"; break;
                    case TextureType::NORMAL:   textureKey = "normalMap";   break;
                    case TextureType::EMISSIVE: textureKey = "emissiveMap"; break;
                    case TextureType::ALPHA:    textureKey = "alphaMap";    break;
                    case TextureType::METALLIC: textureKey = "metallicMap"; break;
                    default:                    textureKey = "diffuseMap";  break;
                }

                // CALL SetTexture WITH THE STRING KEY
                matData->SetTexture(textureKey, texture);
                
                Debug::Log(LogLevel::DEBUG, "[AssimpLoader] Successfully set texture key '%s' (%s)", 
                        textureKey, fullPath.string().c_str());
                return true;
            }
            return false;
        };

        // Try standard DIFFUSE first; if Assimp assigned OBJ map_Kd to BASE_COLOR or UNKNOWN, try those as fallbacks
        if (!loadAndAttachTexture(aiTextureType_DIFFUSE, TextureType::DIFFUSE)) {
            if (!loadAndAttachTexture(aiTextureType_BASE_COLOR, TextureType::DIFFUSE)) {
                loadAndAttachTexture(aiTextureType_UNKNOWN, TextureType::DIFFUSE);
            }
        }

            // Load secondary maps
            loadAndAttachTexture(aiTextureType_SPECULAR,          TextureType::SPECULAR);
            loadAndAttachTexture(aiTextureType_HEIGHT,            TextureType::NORMAL);
            loadAndAttachTexture(aiTextureType_NORMALS,           TextureType::NORMAL);
            loadAndAttachTexture(aiTextureType_EMISSIVE,          TextureType::EMISSIVE);
            loadAndAttachTexture(aiTextureType_OPACITY,           TextureType::ALPHA);
            loadAndAttachTexture(aiTextureType_DIFFUSE_ROUGHNESS, TextureType::METALLIC);
        }
    }
}

        Mesh* outMesh = new Mesh();
        outMesh->vertices = vertices;
        outMesh->indices = indices;
        outMesh->filepath = path;
        
        // Direct field assignment
        outMesh->material = meshMaterial;
        
        // Setup GPU buffers VAO/VBO/EBO
        outMesh->SetupMesh();

        return outMesh;
    }

}