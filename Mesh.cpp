#include "Mesh.hpp"

using namespace ng::Graphics;

Mesh::Mesh() {
	this->vertices = std::vector<Vertex>();
	this->normals = std::vector<glm::vec3>();
	this->texCoords = std::vector<glm::vec2>();
	this->indices = std::vector<unsigned int>();
	this->EBO = 0;
	this->VBO = 0;
	this->VAO = 0;
}

Mesh::Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords, std::vector<unsigned int> inds)
{
    for (size_t i = 0; i < positions.size(); i++) {
        Vertex v;
        v.position = positions[i];
        v.normal = (i < normals.size()) ? normals[i] : glm::vec3(0.0f, 0.0f, 1.0f);
        v.texCoord = (i < texCoords.size()) ? texCoords[i] : glm::vec2(0.0f, 0.0f);
        vertices.push_back(v);
    }

    indices = inds;
    SetupMesh();
}

