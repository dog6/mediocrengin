// objFileParser.cpp
#include <AVGNG/ObjFileParser.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <AVGNG/stb_image.h>

#include <AVGNG/TexCoord.hpp>

using namespace std;
using namespace ng::Core;
using namespace ng::Graphics;

namespace ng::Assets {

    // Helper Methods
    glm::vec3 ProcessVertexPositions(const string& line) {
        stringstream ss(line);
        char v;
        float x, y, z;
        ss >> v >> x >> y >> z;
        return glm::vec3(x, y, z);
    }

    glm::vec3 ProcessVertexNormals(const string& line) {
        stringstream ss(line);
        char v, n;
        float x, y, z;
        ss >> v >> n >> x >> y >> z;
        return glm::vec3(x, y, z);
    }

    TexCoord ProcessTexCoords(const string& line) {
        stringstream ss(line);
        string vt;
        float u, v_coord;
        ss >> vt >> u >> v_coord;
        return { nullptr, nullptr, u, v_coord };
    }

    void ProcessFace(
        const string& line,
        const vector<glm::vec3>& positions,
        const vector<glm::vec2>& texCoords,
        const vector<glm::vec3>& normals,
        vector<Vertex>& vertices,
        vector<unsigned int>& indices,
        unordered_map<string, unsigned int>& vertexCache
    ) {
        // Use stringstream - more reliable than custom split
        stringstream ss(line);
        vector<string> faceGroups;
        string token;

        while (ss >> token) {
            faceGroups.push_back(token);
        }

#ifndef NG_QUIET_PARSING
        Debug::Log(DEBUG, "Face has %d vertex groups", faceGroups.size());
#endif

        if (faceGroups.empty()) {
            Debug::Log(ERROR, "No vertex groups found in face: '%s'", line.c_str());
            return;
        }

        vector<unsigned int> faceIndices;

        for (auto& groupStr : faceGroups) {
            // Check cache
            if (vertexCache.find(groupStr) != vertexCache.end()) {
                faceIndices.push_back(vertexCache[groupStr]);
                continue;
            }

            vector<int> idx = FileReader::split_ints(groupStr, "/");

#ifndef NG_QUIET_PARSING
            Debug::Log(DEBUG, "Parsing '%s' -> %d indices", groupStr.c_str(), idx.size());
#endif

            if (idx.empty() || idx[0] <= 0 || idx[0] > (int)positions.size()) {
                Debug::Log(WARN, "Invalid position index in '%s'", groupStr.c_str());
                continue;
            }

            Vertex v;
            v.position = positions[idx[0] - 1];
            v.texCoord = (idx.size() > 1 && idx[1] > 0 && idx[1] <= (int)texCoords.size())
                ? texCoords[idx[1] - 1] : glm::vec2(0.0f);
            v.normal = (idx.size() > 2 && idx[2] > 0 && idx[2] <= (int)normals.size())
                ? normals[idx[2] - 1] : glm::vec3(0.0f, 0.0f, 1.0f);

            vertices.push_back(v);
            unsigned int newIndex = (unsigned int)vertices.size() - 1;
            vertexCache[groupStr] = newIndex;
            faceIndices.push_back(newIndex);

#ifndef NG_QUIET_PARSING
            Debug::Log(DEBUG, "Created vertex %d", newIndex);
#endif

        }

        // Triangulate
        if (faceIndices.size() >= 3) {
            if (faceIndices.size() == 3) {
                indices.push_back(faceIndices[0]);
                indices.push_back(faceIndices[1]);
                indices.push_back(faceIndices[2]);
            }
            else if (faceIndices.size() == 4) {
                indices.push_back(faceIndices[0]);
                indices.push_back(faceIndices[1]);
                indices.push_back(faceIndices[2]);
                indices.push_back(faceIndices[0]);
                indices.push_back(faceIndices[2]);
                indices.push_back(faceIndices[3]);
            }
            else {
                for (size_t i = 1; i < faceIndices.size() - 1; i++) {
                    indices.push_back(faceIndices[0]);
                    indices.push_back(faceIndices[i]);
                    indices.push_back(faceIndices[i + 1]);
                }
            }
        }
        else {
            Debug::Log(WARN, "Face has less than 3 vertices (%d)", faceIndices.size());
        }
    }

    unsigned int LoadTextureFromFile(const string& path)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, channels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            Debug::Log(ERROR, "Failed to load texture: %s\n", path.c_str());
            return 0;
        }

        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        return textureID;
    }


    // Public Methods
    Mesh* ObjFileParser::LoadObjFromFileAsMesh(const string& objFilePath)
    {
        std::filesystem::path objDir;
        objDir = std::filesystem::path(objFilePath).parent_path();

        ifstream file = FileReader::ReadFile(objFilePath);
        if (!file.is_open()) {
            return nullptr;
        }
        string line;

        vector<Vertex> vertices;
        vector<glm::vec3> positions;
        vector<glm::vec3> normals;
        vector<glm::vec2> texCoords;
        vector<unsigned int> indices;

        string currentMaterial;

        // Make materials static so they persist after function returns
        static unordered_map<string, unordered_map<string, Material>> allMaterials;
        unordered_map<string, Material>& materials = allMaterials[objFilePath];
        unordered_map<string, unsigned int> vertexCache;

        int vertexCount = 0;
        int faceCount = 0;

#ifndef NG_QUIET_PARSING
        Debug::Log(LogLevel::DEBUG, "Loading OBJ file: %s\n", objFilePath.c_str());
#endif

        while (getline(file, line))
        {
            // Skip empty lines and comments
            if (line.empty() || line[0] == '#') continue;

            // Vertex positions
            if (line.rfind("v ", 0) == 0)
            {
                glm::vec3 v = ProcessVertexPositions(line);
                positions.emplace_back(v.x, v.y, v.z);
                vertexCount++;

#ifndef NG_QUIET_PARSING
                Debug::Log(LogLevel::DEBUG, "Loaded vertex %d: (%f, %f, %f)\n", vertexCount, v.x, v.y, v.z);
#endif
            }
            // Vertex normals
            else if (line.rfind("vn ", 0) == 0)
            {
                glm::vec3 vn = ProcessVertexNormals(line);
                normals.emplace_back(vn.x, vn.y, vn.z);
            }
            // Texture coordinates
            else if (line.rfind("vt ", 0) == 0)
            {
                TexCoord tc = ProcessTexCoords(line);
                texCoords.emplace_back(tc.u, tc.v_coord);
            }
            // Faces
            else if (line.rfind("f ", 0) == 0)
            {
                faceCount++;

#ifndef NG_QUIET_PARSING
                Debug::Log(LogLevel::DEBUG, "Processing face %d: %s", faceCount, line.c_str());
#endif

                // Remove the leading "f "
                line = line.substr(2);

                ProcessFace(line, positions, texCoords, normals, vertices, indices, vertexCache);
            }

            // Parse MTL
            if (line.rfind("usemtl ", 0) == 0)
            {
                currentMaterial = line.substr(7);
#ifndef NG_QUIET_PARSING
                Debug::Log(LogLevel::DEBUG, "Switching to material: %s", currentMaterial.c_str());
#endif

            }

            if (line.rfind("mtllib ", 0) == 0) {
                std::string mtlFile = line.substr(7);

                // Trim trailing whitespace
                mtlFile.erase(mtlFile.find_last_not_of(" \r\n\t") + 1);

                std::filesystem::path mtlFullPath = objDir / mtlFile;
                std::string mtlPath = mtlFullPath.string();

                if (materials.empty()) {
                    materials = LoadMaterialFromFile(mtlPath);
                }
                
                // Otherwise material already loaded from previous call

#ifndef NG_QUIET_PARSING
                Debug::Log(LogLevel::DEBUG, "Found %zu materials\n    .obj file '%s'\n    .mtl file '%s'",
                    materials.size(), objFilePath.c_str(), mtlPath.c_str());
#endif

            }

        }

#ifndef NG_QUIET_PARSING
        Debug::Log(LogLevel::DEBUG, "Total: %d positions, %d faces, %d final vertices\n",
            vertexCount, faceCount, (int)vertices.size());
#endif

        file.close();

        if (positions.empty()) {
            Debug::Log(LogLevel::WARN, "WARNING: No vertices loaded from %s", objFilePath.c_str());
            return nullptr;
        }

        // Create and return the mesh
        Mesh* mesh = new Mesh(vertices, indices);

        if (!currentMaterial.empty() && materials.find(currentMaterial) != materials.end()) {
            mesh->material = &materials[currentMaterial];
        }

        if (mesh == nullptr) {
#ifndef NG_QUIET_PARSING
            Debug::Log(LogLevel::DEBUG, "Failed to create mesh from file %s", objFilePath.c_str());
#endif
            return nullptr;
        }
        Debug::Log(LOG, "Successfully loaded .obj file as mesh %s", objFilePath.c_str());

#ifndef NG_QUIET_PARSING
        Debug::Log(DEBUG, "VAO: %d, VBO: %d, EBO: %d", mesh->VAO, mesh->VBO, mesh->EBO);
#endif

        return mesh;
    }

    unordered_map<string, Material> ng::Assets::ObjFileParser::LoadMaterialFromFile(const string& mtlPath)
    {

#ifndef NG_QUIET_PARSING
        Debug::Log(LogLevel::DEBUG, "Loading MTL file: '%s'", mtlPath.c_str());
#endif

        if (!std::filesystem::exists(mtlPath)) {
            Debug::Log(WARN, "MTL file does not exist: '%s'", mtlPath.c_str());
        }

        unordered_map<string, Material> materials;

        ifstream file(mtlPath);
        if (!file.is_open()) {
            Debug::Log(WARN, "Failed to open MTL file: %s", mtlPath.c_str());
            return materials;
        }

        Material* current = nullptr;
        string line;

        while (getline(file, line))
        {
            if (line.empty() || line[0] == '#') continue;

            stringstream ss(line);
            string keyword;
            ss >> keyword;

            if (keyword == "newmtl")
            {
                string name;
                ss >> name;
                materials[name] = Material();
                materials[name].name = name;
                current = &materials[name];
            }
            else if (!current)
            {
                continue;
            }
            else if (keyword == "Ka")
            {
                ss >> current->Ka.r >> current->Ka.g >> current->Ka.b;
            }
            else if (keyword == "Kd")
            {
                ss >> current->Kd.r >> current->Kd.g >> current->Kd.b;
            }
            else if (keyword == "Ks")
            {
                ss >> current->Ks.r >> current->Ks.g >> current->Ks.b;
            }
            else if (keyword == "Ns")
            {
                ss >> current->Ns;
            }
            else if (keyword == "map_Kd")
            {
                string texFile;
                ss >> texFile;

                std::filesystem::path texFullPath = std::filesystem::path(mtlPath).parent_path() / texFile;
                current->diffuseTexID = LoadTextureFromFile(texFullPath.string());
            }
        }

        Debug::Log(LOG, "Loaded %zu materials from %s", materials.size(), mtlPath.c_str());
        return materials;
    }

    GameObject* ObjFileParser::LoadObjAsGameObject(const char* name, const char* objFilePath, Shader* shader) {

        if (!filesystem::exists(objFilePath)) {
            Debug::Log(ERROR, "Failed to load .obj with path '%s'.", objFilePath);
        }

        GameObject* result = new GameObject(name);

        result->AddComponent<MeshRenderer>();
        result->AddComponent<Transform>();

        MeshRenderer* resultMeshRenderer = result->GetComponent<MeshRenderer>();

        ObjFileParser parser = ObjFileParser();
        resultMeshRenderer->SetMesh(parser.LoadObjFromFileAsMesh(objFilePath));

        if (shader != nullptr) {
            resultMeshRenderer->shader = shader;
        }
        else if (Shader* defaultShader = ShaderLoader::LoadDefaultShader()) {
            resultMeshRenderer->shader = defaultShader;

            if (defaultShader == nullptr) {
                Debug::Log(ERROR, "Failed to load shader while loading .obj as gameObject.\n.OBJ path: '%s'.", objFilePath);
            }
        }

        return result;

    }

}