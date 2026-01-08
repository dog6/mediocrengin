#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <glad/glad.h>

namespace ng {
namespace Graphics {

#ifndef VERTEX_STRUCT
#define VERTEX_STRUCT
    struct Vertex {
        glm::vec3 position;  // Where the point is in 3D space (x, y, z)
        glm::vec3 normal;    // Which direction it's facing (for lighting)
        glm::vec2 texCoord;  // Where on a texture image this point maps to
    };
#endif

#ifndef MESH_HPP
#define MESH_HPP
	class Mesh {

	public:
        Mesh();
		Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords, std::vector<unsigned int> indices);
        ~Mesh() {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }
		std::vector<Vertex> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		std::vector<unsigned int> indices;

		unsigned int VAO;  // Vertex Array Object
		unsigned int VBO;  // Vertex Buffer Object
		unsigned int EBO;  // Element Buffer Object (for indices)

        void SetupMesh()
        {
            printf("SetupMesh called with %d vertices, %d indices\n",
                (int)vertices.size(), (int)indices.size());

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            printf("Generated VAO: %d, VBO: %d, EBO: %d\n", VAO, VBO, EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                &indices[0], GL_STATIC_DRAW);

            // Position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                (void*)offsetof(Vertex, position));
            glEnableVertexAttribArray(0);

            // Normal attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                (void*)offsetof(Vertex, normal));
            glEnableVertexAttribArray(1);

            // TexCoord attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                (void*)offsetof(Vertex, texCoord));
            glEnableVertexAttribArray(2);

            glBindVertexArray(0);

            printf("SetupMesh completed\n");
        }

        void Draw() {
            if (indices.size() == 0) return;
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

  

	};

    #endif


}}
