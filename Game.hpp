#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include<glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Shader.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "ObjFileParser.hpp"
#include "Renderer.hpp"
#include "Transform.hpp"
#include "GameObject.hpp"

#define FRAG_SHADER_PATH = G:/Projects/NG/AvgNGin/res/shaders/fragShaders/
#define VERT_SHADER_PATH = G:/Projects/NG/AvgNGin/res/shaders/vertShaders/

namespace ng {
	class Game {
		std::string windowTitle;
		glm::uvec2 windowSize;
	public:
		Game();
		~Game();
		void Load();    // Called before game starts
		void Start();    // Called when game first starts
		void Run();    // Called after Start(), GameLoop
		void Exit();    // Called when game is closed
	};
}
#endif