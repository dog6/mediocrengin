

#include <stb_image.h>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "AVGNG/Graphics/Skybox.hpp"
#include "AVGNG/Core/Debug.hpp"
using namespace ng::Core;

namespace ng::Graphics {

	float skyboxVertices[] = {
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f
	};

	unsigned int skyboxIndices[] = {
		// Right
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	};
	
	
	Skybox::Skybox(glm::uvec2& viewportSize) : size(viewportSize),
		lightColor(glm::vec3(1.0f, 0.95f, 0.8f)),
		lightPos(glm::normalize(glm::vec3(-0.3f, -1.0f, -0.5f))),
		skyboxEBO(0),
		skyboxVAO(0),
		skyboxVBO(0),
		cubemap(CubemapTexture()) { }

	Skybox::~Skybox() {

	}

	void Skybox::Init() {

		Debug::Log(DEBUG, "Initializing skybox..");

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		// Create VAO, VBO & EBO
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glGenBuffers(1, &skyboxEBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		std::string facesCubemap[6] = {
			cubemap.right_texture_path,
			cubemap.left_texture_path,
			cubemap.top_texture_path,
			cubemap.bottom_texture_path,
			cubemap.front_texture_path,
			cubemap.back_texture_path
		};


		glGenTextures(1, &cubemap.textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.textureID);
		
		stbi_set_flip_vertically_on_load(false);

		for (unsigned int i = 0; i < 6; i++) {

			int width, height, nrChannels;
			unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
			if (data) {
				glTexImage2D
				(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0,
					GL_SRGB8,
					width,
					height,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					data
				);
				stbi_image_free(data);
				Debug::Log(DEBUG, "Loaded skybox texture: %s", facesCubemap[i].c_str());
			}
			else {
				Debug::Log(ERROR, "Failed to load skybox texture: %s", facesCubemap[i].c_str());
				stbi_image_free(data);
				throw std::runtime_error("Failed to load skybox texture");
			}

		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		Debug::Log(DEBUG, "Created skybox cubemap with texture ID: %d", cubemap.textureID);

		Debug::Log(DEBUG, "Loading skybox shader..");

		// Load skybox shader
		skyboxShader = ng::Assets::ShaderLoader::LoadShader("Skybox Shader", "D:/Projects/CPP/smallengine/res/shaders/vertexShaders/shader_skybox.vert", "D:/Projects/CPP/smallengine/res/shaders/fragShaders/shader_skybox.frag");
		if (!&skyboxShader) {
			ng::Core::Debug::Log(ng::Core::LogLevel::ERROR, "Failed to load shader for skybox");
			return;
		}
		Debug::Log(DEBUG, "Successfully loaded shader for skybox");

		Debug::Log(DEBUG, "Skybox Initialized");
	}

	void Skybox::Render(Camera& camera)
	{
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Use();

		// Get the camera's view matrix and remove translation
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));

		// Use the camera's projection matrix
		glm::mat4 projection = camera.GetProjectionMatrix(size.x, size.y);

		skyboxShader.SetMat4("view", view);
		skyboxShader.SetMat4("projection", projection);
		skyboxShader.SetInt("skybox", 0);

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.textureID);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}

}