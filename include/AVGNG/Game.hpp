#pragma once

#define STB_IMAGE_IMPLEMENTATION

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

#include <AVGNG/Shader.hpp>
#include <AVGNG/Camera.hpp>
#include <AVGNG/Mesh.hpp>
#include <AVGNG/MeshRenderer.hpp>
#include <AVGNG/ObjFileParser.hpp>
#include <AVGNG/Renderer.hpp>
#include <AVGNG/Transform.hpp>
#include <AVGNG/GameObject.hpp>
#include <AVGNG/Debug.hpp>
#include <AVGNG/Time.hpp>
#include <AVGNG/Scene.hpp>

#define FRAG_SHADER_PATH = G:/Projects/NG/AvgNGin/res/shaders/fragShaders/
#define VERT_SHADER_PATH = G:/Projects/NG/AvgNGin/res/shaders/vertShaders/



namespace ng::Core {

		class Game {
			const char* windowTitle;
			glm::uvec2 windowSize;

		public:
			Game();
			Game(const char* title, glm::uvec2 size);
			~Game();
			void Load();    // Called before game starts
			void Start();    // Called when game first starts
			void Run();    // Called after Start(), GameLoop
			void Exit();    // Called when game is closed

		};

}
