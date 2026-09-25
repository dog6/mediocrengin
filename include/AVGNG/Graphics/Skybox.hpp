#pragma once

#include "AVGNG/Graphics/Camera.hpp"
#include "AVGNG/Graphics/ShaderLoader.hpp"
#include "AVGNG/Graphics/Shader.hpp"
#include "AVGNG/Graphics/CubemapTexture.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ng::Graphics {



	// TODO: Finish implementing skybox
	class Skybox {
	private:
		unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
		ng::Graphics::Shader skyboxShader;
		ng::Graphics::CubemapTexture cubemap;
		//unsigned int skybox_textureID;

		glm::uvec2& size;
		glm::vec3 lightColor;
		glm::vec3 lightPos;
	public:
		Skybox(glm::uvec2& viewportSize);
		~Skybox();
		void Init();
		void Render(ng::Graphics::Camera& camera);
	};

}