// objFileParser.cpp
#include <AVGNG/ObjFileParser.hpp>

#ifndef STB_IMAGE_IMPLEMENTATION
    #include <AVGNG/stb_image.h>
#endif

#include <AVGNG/TexCoord.hpp>
#include <AVGNG/Utility.hpp>


using namespace std;
using namespace ng::Core;
using namespace ng::Graphics;

namespace ng::Assets {


    // Helper Method
    
    std::string GetFullPathOfTexture(const std::string& textureFile, std::stringstream& ss) {
        std::string textureFilename;
        ss >> textureFilename;
        ng::Assets::Utility::GetFullPathFromFilename(textureFile, textureFilename);
        return textureFilename;
    }

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
        const std::string& line,
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec2>& texCoords,
        const std::vector<glm::vec3>& normals,
        std::vector<Vertex>& vertices,
        std::vector<unsigned int>& indices,
        std::unordered_map<std::string, unsigned int>& vertexCache
    ) {
        std::stringstream ss(line);
        std::vector<std::string> faceGroups;
        std::string token;

        while (ss >> token)
            faceGroups.push_back(token);

        if (faceGroups.empty()) {
            Debug::Log(ERROR, "No vertex groups found in face: '%s'", line.c_str());
            return;
        }

        std::vector<unsigned int> faceIndices;

        for (auto& groupStr : faceGroups) {
            // Check cache
            if (vertexCache.find(groupStr) != vertexCache.end()) {
                faceIndices.push_back(vertexCache[groupStr]);
                continue;
            }

            // Split vertex/uv/normal indices
            std::vector<int> idx = FileReader::split_ints(groupStr, "/");
            Vertex v;

            // Position
            if (idx.empty() || idx[0] <= 0 || idx[0] > (int)positions.size()) {
                Debug::Log(WARN, "Invalid position index in '%s'", groupStr.c_str());
                continue;
            }
            v.position = positions[idx[0] - 1];

            // TexCoord
            if (idx.size() > 1 && idx[1] > 0 && idx[1] <= (int)texCoords.size())
                v.texCoord = texCoords[idx[1] - 1];
            else
                v.texCoord = glm::vec2(0.0f, 0.0f);

            // Normal
            if (idx.size() > 2 && idx[2] > 0 && idx[2] <= (int)normals.size()) {
                v.normal = normals[idx[2] - 1];
            }
            else {
                // Will compute face normal later if missing
                v.normal = glm::vec3(0.0f);
            }

            vertices.push_back(v);
            unsigned int newIndex = (unsigned int)vertices.size() - 1;
            vertexCache[groupStr] = newIndex;
            faceIndices.push_back(newIndex);
        }

        // If normals were missing, compute per-face normal
        bool missingNormals = false;
        for (auto idx : faceIndices)
            if (vertices[idx].normal == glm::vec3(0.0f))
                missingNormals = true;

        if (missingNormals && faceIndices.size() >= 3) {
            glm::vec3 a = vertices[faceIndices[0]].position;
            glm::vec3 b = vertices[faceIndices[1]].position;
            glm::vec3 c = vertices[faceIndices[2]].position;
            glm::vec3 faceNormal = glm::normalize(glm::cross(b - a, c - a));
            for (auto idx : faceIndices)
                vertices[idx].normal = faceNormal;
        }

        // Triangulate if face has more than 3 vertices (fan)
        for (size_t i = 1; i + 1 < faceIndices.size(); ++i) {
            indices.push_back(faceIndices[0]);
            indices.push_back(faceIndices[i]);
            indices.push_back(faceIndices[i + 1]);
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


    // Load texture from file and return OpenGL texture ID
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
        Debug::Log(LOG, "Loaded texture ID '%d' from file '%s'", textureID, path.c_str());
        return textureID;
    }


    void ParseOBJFile(ifstream& file, string& line,
        vector<glm::vec3>& positions, vector<glm::vec3>& normals,
        vector<glm::vec2>& texCoords, vector<Vertex>& vertices,
        vector<unsigned int>& indices, unordered_map<string, unsigned int>& vertexCache,
        int& vertexCount, int& faceCount) {
        Debug::Log(DEBUG, "Parsing OBJ file..");

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
        }
    }

    void ParseMTLFile(std::unordered_map<std::string, MaterialData>& materials, std::ifstream& file, std::string& line, const std::string& mtlPath)
    {
		Debug::Log(DEBUG, "Parsing MTL file: '%s'..", mtlPath.c_str());
        MaterialData* current = nullptr;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            std::stringstream ss(line);
            std::string keyword;
            ss >> keyword;

            if (keyword == "newmtl") {
                std::string name;
                ss >> name;
                materials[name] = MaterialData();
                materials[name].name = name;
                current = &materials[name];
                Debug::Log(DEBUG, "Found new material: %s", name.c_str());
            }
            else if (!current) continue;

            else if (keyword == "Ka") ss >> current->Ambient.r >> current->Ambient.g >> current->Ambient.b;
            else if (keyword == "Kd") ss >> current->Diffuse.r >> current->Diffuse.g >> current->Diffuse.b;
            else if (keyword == "Ks") ss >> current->Specular.r >> current->Specular.g >> current->Specular.b;
            else if (keyword == "Ke") ss >> current->Emissive.r >> current->Emissive.g >> current->Emissive.b;
            else if (keyword == "Ni") ss >> current->IOR;
            else if (keyword == "Ns") ss >> current->Shininess;

            else if (keyword == "map_Kd") {
                std::string texPath = GetFullPathOfTexture(mtlPath, ss);
                current->diffuseTexID = LoadTextureFromFile(texPath);
                Debug::Log(DEBUG, "!! Loaded diffuse texture for material '%s' from '%s'", current->name.c_str(), texPath.c_str());
            }
            else if (keyword == "map_Ks") {
                std::string texPath = GetFullPathOfTexture(mtlPath, ss);
                current->specularTexID = LoadTextureFromFile(texPath);
                Debug::Log(DEBUG, "!! Loaded specular texture for material '%s' from '%s'", current->name.c_str(), texPath.c_str());

            }
            else if (keyword == "map_Ke") {
                std::string texPath = GetFullPathOfTexture(mtlPath, ss);
                current->emissiveTexID = LoadTextureFromFile(texPath);
                Debug::Log(DEBUG, "!! Loaded emissive texture for material '%s' from '%s'", current->name.c_str(), texPath.c_str());

            }
            else if (keyword == "map_d") {
                std::string texPath = GetFullPathOfTexture(mtlPath, ss);
                current->alphaTexID = LoadTextureFromFile(texPath);
                Debug::Log(DEBUG, "!! Loaded displacement texture for material '%s' from '%s'", current->name.c_str(), texPath.c_str());
            }
            else if (keyword == "map_Bump" || keyword == "bump") {
                std::string texPath = GetFullPathOfTexture(mtlPath, ss);
                current->normalTexID = LoadTextureFromFile(texPath);
                Debug::Log(DEBUG, "!! Loaded normal texture for material '%s' from '%s'", current->name.c_str(), texPath.c_str());
            }
        }
    }

    // Public Methods
    Mesh* ObjFileParser::LoadObjFromFileAsMesh(const std::string& objFilePath) {
        
        std::ifstream file(objFilePath);
        if (!file.is_open()) {
            Debug::Log(ERROR, "Failed to open OBJ file: %s", objFilePath.c_str());
            return nullptr;
        }

        std::filesystem::path objDir = std::filesystem::path(objFilePath).parent_path();
        std::vector<Vertex> vertices;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        std::vector<unsigned int> indices;
        std::unordered_map<std::string, unsigned int> vertexCache;

        std::unordered_map<std::string, MaterialData> materials;
        std::string currentMaterial;

        std::string line;
        int vertexCount = 0;
        int faceCount = 0;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            if (line.rfind("mtllib ", 0) == 0) {
                std::string mtlFile = line.substr(7);
                mtlFile.erase(mtlFile.find_last_not_of(" \r\n\t") + 1);
                materials = LoadMaterialsFromFile((objDir / mtlFile).string());
            }
            else if (line.rfind("usemtl ", 0) == 0) {
                currentMaterial = line.substr(7);
                currentMaterial.erase(currentMaterial.find_last_not_of(" \r\n\t") + 1);
            }
            else if (line.rfind("v ", 0) == 0) positions.push_back(ProcessVertexPositions(line)), vertexCount++;
            else if (line.rfind("vn ", 0) == 0) normals.push_back(ProcessVertexNormals(line));
            else if (line.rfind("vt ", 0) == 0) {
                TexCoord tc = ProcessTexCoords(line);
                texCoords.push_back(glm::vec2(tc.u, tc.v_coord));
            }
            else if (line.rfind("f ", 0) == 0) {
                line = line.substr(2);
                ProcessFace(line, positions, texCoords, normals, vertices, indices, vertexCache);
            }
        }
        file.close();

        if (positions.empty()) {
            Debug::Log(WARN, "No vertices loaded from %s", objFilePath.c_str());
            return nullptr;
        }

        Mesh* mesh = new Mesh(vertices, indices);
        mesh->filepath = objFilePath.c_str();

        if (!currentMaterial.empty() && materials.find(currentMaterial) != materials.end()) {
            mesh->material->SetMaterialData(materials[currentMaterial]);
            Debug::Log(LOG, "Applied material '%s' to mesh '%s'", materials[currentMaterial].name.c_str(), mesh->filepath);
        }

        return mesh;
    }

    // Load MTL file
    std::unordered_map<std::string, MaterialData> ng::Assets::ObjFileParser::LoadMaterialsFromFile(const std::string& mtlPath) {
        std::unordered_map<std::string, MaterialData> materials;

        if (!std::filesystem::exists(mtlPath)) {
            Debug::Log(WARN, "MTL file does not exist: '%s'", mtlPath.c_str());
            return materials;
        }

        std::ifstream file(mtlPath);
        if (!file.is_open()) {
            Debug::Log(WARN, "Failed to open MTL file: %s", mtlPath.c_str());
            return materials;
        }

        std::string line;
		ParseMTLFile(materials, file, line, mtlPath); // populates materials map

        file.close();

        if (materials.size() != 0) {
            Debug::Log(LOG, "Loaded %zu materials from %s", materials.size(), mtlPath.c_str());
        }
        else {
            Debug::Log(WARN, "Failed to find materials from MTL file: '%s'", mtlPath.c_str());
        }
        return materials;
    }
    
    // Load .obj as GameObject ( starts w/ Transform & MeshRenderer components attached )
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