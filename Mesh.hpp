#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <glad/glad.h>
#include "Vertex.hpp"
#include "Material.hpp"
#include "Debug.hpp"
#include "Shader.hpp"
#include "Transform.hpp"
#include "Camera.hpp"



namespace ng {
namespace Graphics {

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
        //Vertex shaep
		std::vector<Vertex> vertices;
        std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		std::vector<unsigned int> indices;
        Shader* shader;

		unsigned int VAO;  // Vertex Array Object
		unsigned int VBO;  // Vertex Buffer Object
		unsigned int EBO;  // Element Buffer Object (for indices)

        Material* material;

        void SetupMesh();

        void Draw(Camera* camera, Transform* transform);

  

	};



}}
