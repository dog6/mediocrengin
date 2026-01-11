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

#include <AVGNG/KeyboardInput.hpp>

// Graphics
#include <AVGNG/Graphics.hpp>

// Core
#include <AVGNG/Core.hpp>

// Scripting
#include <AVGNG/LuaManager.hpp>

// Assets
#include <AVGNG/ObjFileParser.hpp>
#include <AVGNG/ShaderLoader.hpp>


namespace ng::Core {

		class Game {
			const char* windowTitle;
			glm::uvec2 windowSize;

		public:
			Game();
			Game(const char* title, glm::uvec2 size);
			~Game();
			void Init();    // Called when game first starts up
			void Load();    // Called before game loop starts
			void Start();    // Called when game first starts
			void Run();    // Called after Start(), GameLoop
			void Exit();    // Called when game is closed

		};

}
