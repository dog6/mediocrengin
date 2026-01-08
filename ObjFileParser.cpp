// objFileParser.cpp
#include "objFileParser.hpp"
#include "FileReader.hpp"

using namespace ng::Assets;
using namespace ng::Graphics;

Mesh* ObjFileParser::LoadObjFromFile(const std::string& objFilePath)
{

    std::ifstream file = FileReader::ReadFile(objFilePath);
    if (!file.is_open()) {
        return nullptr;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;

    std::string line;
    int vertexCount = 0;
    int faceCount = 0;

    printf("Loading OBJ file: %s\n", objFilePath.c_str());

    while (std::getline(file, line))
    {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;

        // Vertex positions
        if (line.rfind("v ", 0) == 0)
        {
            std::stringstream ss(line);
            char v;
            float x, y, z;
            ss >> v >> x >> y >> z;
            positions.emplace_back(x, y, z);
            vertexCount++;
            printf("Loaded vertex %d: (%f, %f, %f)\n", vertexCount, x, y, z);
        }
        // Vertex normals
        else if (line.rfind("vn ", 0) == 0)
        {
            std::stringstream ss(line);
            char v, n;
            float x, y, z;
            ss >> v >> n >> x >> y >> z;
            normals.emplace_back(x, y, z);
        }
        // Texture coordinates
        else if (line.rfind("vt ", 0) == 0)
        {
            std::stringstream ss(line);
            char v, t;
            float u, v_coord;
            ss >> v >> t >> u >> v_coord;
            texCoords.emplace_back(u, v_coord);
        }
        // Faces
        else if (line.rfind("f ", 0) == 0)
        {
            faceCount++;
            printf("Processing face %d: %s\n", faceCount, line.c_str());

            // Remove the leading "f "
            line = line.substr(2);
            std::vector<std::string> faceIndexGroups = FileReader::split(line, ' ');

            printf("  Face has %zu indices\n", faceIndexGroups.size());

            std::vector<Vertex> faceVertices;
            for (size_t i = 0; i < faceIndexGroups.size(); i++)
            {
                printf("  Processing index group: '%s'\n", faceIndexGroups[i].c_str());

                std::vector<int> indexGroup = FileReader::split_ints(faceIndexGroups[i], "/");
                if (indexGroup.empty()) {
                    printf("  WARNING: Empty index group!\n");
                    continue;
                }

                int posIndex = indexGroup[0] - 1;
                printf("  Point index: %d (from %d positions)\n", posIndex, (int)positions.size());

                if (posIndex < 0 || posIndex >= (int)positions.size()) {
                    printf("  ERROR: Index out of bounds!\n");
                    continue;
                }

                Vertex vertex;
                vertex.position = positions[posIndex];

                // Handle texture coordinates if present
                if (indexGroup.size() > 1 && indexGroup[1] > 0 && indexGroup[1] <= (int)texCoords.size()) {
                    vertex.texCoord = texCoords[indexGroup[1] - 1];
                }
                else {
                    vertex.texCoord = glm::vec2(0.0f, 0.0f);
                }

                // Handle normals if present
                if (indexGroup.size() > 2 && indexGroup[2] > 0 && indexGroup[2] <= (int)normals.size()) {
                    vertex.normal = normals[indexGroup[2] - 1];
                }
                else {
                    vertex.normal = glm::vec3(0.0f, 0.0f, 1.0f);
                }

                faceVertices.push_back(vertex);
            }

            // Triangulate (fan triangulation for n-gons)
            if (faceVertices.size() >= 3) {
                for (size_t i = 1; i < faceVertices.size() - 1; i++)
                {
                    unsigned int baseIndex = (unsigned int)positions.size();

                    positions.push_back(faceVertices[0].position);
                    positions.push_back(faceVertices[i].position);
                    positions.push_back(faceVertices[i + 1].position);

                    // Add indices
                    indices.push_back(baseIndex);
                    indices.push_back(baseIndex + 1);
                    indices.push_back(baseIndex + 2);
                }
                printf("  Generated %d triangles from this face\n", (int)(faceVertices.size() - 2));
            }
        }
    }

    printf("Total: %d positions, %d faces, %d final vertices\n",
        vertexCount, faceCount, (int)positions.size());

    file.close();

    if (positions.empty()) {
        printf("WARNING: No vertices loaded from %s\n", objFilePath.c_str());
        return nullptr;
    }

    // Create and return the mesh
    Mesh* mesh = new Mesh(positions, normals, texCoords, indices);

    if (mesh == nullptr) {
        printf("Failed to create mesh from file %s\n", objFilePath.c_str());
        return nullptr;
    }
    printf("VAO: %d, VBO: %d, EBO: %d\n", mesh->VAO, mesh->VBO, mesh->EBO);

    printf("Successfully loaded .obj file as mesh %s\n", objFilePath.c_str());
    mesh->SetupMesh();
    return mesh;
}