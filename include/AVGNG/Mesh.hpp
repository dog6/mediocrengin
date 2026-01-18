#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <glad/glad.h>
#include <AVGNG/Vertex.hpp>
#include <AVGNG/Material.hpp>
#include <AVGNG/Debug.hpp>
#include <AVGNG/Shader.hpp>

#include <AVGNG/Transform.hpp>
#include <AVGNG/Camera.hpp>


namespace ng::Graphics {

        class Mesh {

        public:
            Mesh();
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
            Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords, std::vector<unsigned int> inds);

            ~Mesh() {
                glDeleteVertexArrays(1, &VAO);
                glDeleteBuffers(1, &VBO);
                glDeleteBuffers(1, &EBO);
            }

            std::string filepath;

            // Vertex shape
            std::vector<Vertex> vertices;
            std::vector<glm::vec3> positions;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> texCoords;
            std::vector<unsigned int> indices;

            Material* material = new Material();

            unsigned int VAO;  // Vertex Array Object
            unsigned int VBO;  // Vertex Buffer Object
            unsigned int EBO;  // Element Buffer Object (for indices)


            void SetupMesh();

			void UseShader(ng::Graphics::Camera& camera, ng::Core::Transform& transform);

            Material* GetMaterial() { return material; }

        };


}