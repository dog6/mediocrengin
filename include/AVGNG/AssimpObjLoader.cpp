#define STB_IMAGE_IMPLEMENTATION
#include <AVGNG/stb_image.h>

#include <AVGNG/AssimpObjLoader.hpp>
#include <AVGNG/Debug.hpp>

#include <filesystem>
#include <iostream>


namespace ng::Assets {

    // Texture Loader
    Texture* AssimpObjLoader::LoadTextureFromFile(const string& path) {

        Texture* tex = new Texture();

        int nrChannels;
        tex->data = stbi_load(path.c_str(), &tex->width, &tex->height, &nrChannels, 0);
        if (!tex->data) {
            Debug::Log(ERROR, "Failed to load texture: %s", path.c_str());
            delete tex;
            return nullptr;
        }

        // Determine internal format
        if (nrChannels == 1) { tex->internalFormat = GL_R8; tex->dataFormat = GL_RED; }
        else if (nrChannels == 3) { tex->internalFormat = GL_RGB8; tex->dataFormat = GL_RGB; }
        else if (nrChannels == 4) { tex->internalFormat = GL_RGBA8; tex->dataFormat = GL_RGBA; }
        else {
            Debug::Log(ERROR, "Unsupported texture channels: %d", nrChannels);
            stbi_image_free(tex->data);
            delete tex;
            return nullptr;
        }

        // Generate OpenGL texture
        glGenTextures(1, &tex->id);
        glBindTexture(GL_TEXTURE_2D, tex->id);

        // Texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Alignment for any width
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Upload to GPU
        glTexImage2D(GL_TEXTURE_2D, 0, tex->internalFormat, tex->width, tex->height,
            0, tex->dataFormat, GL_UNSIGNED_BYTE, tex->data);

        glGenerateMipmap(GL_TEXTURE_2D);

        // Free CPU memory
        stbi_image_free(tex->data);
        tex->data = nullptr;

        Debug::Log(DEBUG, "Loaded texture %s -> ID %u", path.c_str(), tex->id);

        return tex;
  //      filesystem::path texture_file(path);
  //      std::string texture_fullpath = absolute(texture_file).string();

		//Debug::Log(DEV, "Loading texture from %s", texture_fullpath.c_str());

  //      if (!filesystem::exists(texture_file)) {
  //          Debug::Log(ERROR, "Failed to load texture from '%s'", texture_fullpath);
  //          return 0;
  //      }

  //      Debug::Log(DEBUG, "Attempting to load texture from '%s'", texture_fullpath.c_str());

  //      unsigned int textureID;
  //      glGenTextures(1, &textureID);

  //      int width, height, nrChannels;
  //     
  //      // May cause issues
  //      stbi_set_flip_vertically_on_load(true);

  //      // Load texture image data
  //      unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  //      if (data) {
  //          GLenum format = GL_RGB;
  //          if (nrChannels == 1) format = GL_RED;
  //          else if (nrChannels == 4) format = GL_RGBA;
  //          Debug::Log(DEBUG, "Binding to textureID: %d", textureID);
  //          glBindTexture(GL_TEXTURE_2D, textureID);
  //          glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  //          glGenerateMipmap(GL_TEXTURE_2D);

  //          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  //          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //          stbi_image_free(data);
  //          Debug::Log(LOG, "Successfully loaded texture ID: %d", textureID);
  //      }
  //      else {
  //          Debug::Log(ERROR, "STB Failed to decode: %s", path.c_str());
  //          glDeleteTextures(1, &textureID);
  //          return 0;
  //      }

  //      return textureID;
    }


    // Process a single Assimp mesh
    AssimpMesh AssimpObjLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, const string& directory) {
        AssimpMesh result;

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            Vertex vertex;
            vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.normal = mesh->HasNormals() ?
                glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) :
                glm::vec3(0.0f);

            if (mesh->mTextureCoords[0]) {
                vertex.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y);
            }
            else vertex.texCoord = glm::vec2(0.0f, 0.0f);

            result.vertices.push_back(vertex);
        }

        // Indices
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j)
                result.indices.push_back(face.mIndices[j]);
        }

        // Textures
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            auto loadTextures = [&](aiTextureType type, const std::string& texType) {
                for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
                    aiString str;
                    material->GetTexture(type, i, &str);
                    Texture* tex = LoadTextureFromFile(directory + "/" + str.C_Str());
                    result.textures.push_back(*tex);
                }
            };

            loadTextures(aiTextureType_DIFFUSE, "diffuseMap");
            loadTextures(aiTextureType_SPECULAR, "specularMap");
            loadTextures(aiTextureType_HEIGHT, "normalMap");
            loadTextures(aiTextureType_EMISSIVE, "emissiveMap");
            loadTextures(aiTextureType_OPACITY, "alphaMap");
        }

        return result;
    }


    // Recursively process nodes
    void AssimpObjLoader::ProcessNode(aiNode* node, const aiScene* scene, vector<AssimpMesh>& meshes, const string& directory) {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(mesh, scene, directory));
        }
        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            ProcessNode(node->mChildren[i], scene, meshes, directory);
        }
    }


    // Load model from file
    vector<AssimpMesh> AssimpObjLoader::LoadModel(const string& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_CalcTangentSpace
        );

        if (!scene || !scene->mRootNode) {
            Debug::Log(ERROR, "Assimp failed to load %s: %s", path.c_str(), importer.GetErrorString());
            return {};
        }

        vector<AssimpMesh> meshes;
        string directory = filesystem::path(path).parent_path().string();
        ProcessNode(scene->mRootNode, scene, meshes, directory);
        return meshes;
    }


    // AssimpMesh --> Mesh
    Mesh* AssimpObjLoader::LoadObjAsMesh(const string& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            //aiProcess_FlipUVs |       // flip once
            aiProcess_GenNormals |
            aiProcess_CalcTangentSpace);

        if (!scene || !scene->HasMeshes()) {
            Debug::Log(ERROR, "Failed to load OBJ: %s", path.c_str());
            return nullptr;
        }

        vector<Vertex> vertices;
        vector<unsigned int> indices;
        MaterialData mat_data;
        filesystem::path objDir = filesystem::path(path).parent_path();
        unsigned int indexOffset = 0;

        for (unsigned int m = 0; m < scene->mNumMeshes; ++m) {
            aiMesh* mesh = scene->mMeshes[m];

            // --- Vertices ---
            for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
                Vertex v;
                v.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
                v.normal = mesh->HasNormals() ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)
                    : glm::vec3(0.0f, 0.0f, 1.0f);

                v.texCoord = mesh->HasTextureCoords(0)
                    ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) // already flipped in stbi
                    : glm::vec2(0.0f, 0.0f);

                vertices.push_back(v);
            }

            // --- Indices ---
            for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
                aiFace face = mesh->mFaces[f];
                for (unsigned int idx = 0; idx < face.mNumIndices; ++idx)
                    indices.push_back(face.mIndices[idx] + indexOffset);
            }
            indexOffset += mesh->mNumVertices;

            // --- Material & Textures ---
            if (scene->HasMaterials() && mesh->mMaterialIndex >= 0) {
                
                // Load aiMaterial from scene mMaterials
                aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];
				if (ai_material == nullptr) {
					Debug::Log(WARN, "Mesh has no material assigned.");
					continue;
				}

                // Get texture paths
                aiString texPath;

                auto loadTexture = [&](aiTextureType assimpType, const char* ngType) {
                    
                    if (ai_material->GetTextureCount(assimpType) == 0) {
                        Debug::Log(WARN, "No texture of type %d found", assimpType);
                        return;
                    }

                    if (ai_material->GetTexture(assimpType, 0, &texPath) != AI_SUCCESS) return;

                    unsigned int texID = 0;
                    if (texPath.C_Str()[0] == '*') {
                        Debug::Log(WARN, "Embedded texture skipped (not supported): %s", texPath.C_Str());
                        return;
                    }

                    filesystem::path fullPath = objDir / texPath.C_Str();
                    //texID = LoadTextureFromFile(fullPath.string());
					Texture* texture = LoadTextureFromFile(fullPath.string());
                    if (texture == nullptr) {
                        Debug::Log(ERROR, "Failed to load texture '%s'", fullPath.string().c_str());
                        return;
                    }
					Debug::Log(DEBUG, "Loaded texture ID: '%d' type '%s'", texture->id, ngType);
                    mat_data.SetTexture(ngType, texture);

                };

                loadTexture(aiTextureType_DIFFUSE, TEXTURE_TYPE_DIFFUSE);
                loadTexture(aiTextureType_SPECULAR, TEXTURE_TYPE_SPECULAR);
                loadTexture(aiTextureType_HEIGHT, TEXTURE_TYPE_NORMAL);
                loadTexture(aiTextureType_EMISSIVE, TEXTURE_TYPE_EMISSIVE);
                loadTexture(aiTextureType_OPACITY, TEXTURE_TYPE_ALPHA);

                // --- Material colors ---
                aiColor3D color;
                if (ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
                    mat_data.Diffuse = glm::vec3(color.r, color.g, color.b);
                    mat_data.Albedo = mat_data.Diffuse; // fallback tint
                }
                if (ai_material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
                    mat_data.Ambient = glm::vec3(color.r, color.g, color.b);
                if (ai_material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
                    mat_data.Specular = glm::vec3(color.r, color.g, color.b);
                if (ai_material->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS)
                    mat_data.Emissive = glm::vec3(color.r, color.g, color.b);

                float shininess;
                if (ai_material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
                    mat_data.Shininess = shininess;
            }
        }

        Mesh* newMesh = new Mesh(vertices, indices);
        newMesh->material->SetMaterialData(mat_data);
        return newMesh;
    }


}
