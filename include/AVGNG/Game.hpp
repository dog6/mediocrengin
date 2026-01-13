#pragma once

#define STB_IMAGE_IMPLEMENTATION

// Dependents
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

// Inputs
#include <AVGNG/KeyboardInput.hpp>
#include <AVGNG/MouseInput.hpp>

// Graphics
#include <AVGNG/Camera.hpp>
#include <AVGNG/Shader.hpp>
#include <AVGNG/Mesh.hpp>
#include <AVGNG/MeshRenderer.hpp>
#include <AVGNG/Renderer.hpp>

// Core
#include <AVGNG/Time.hpp>
#include <AVGNG/Transform.hpp>
#include <AVGNG/GameObject.hpp>
#include <AVGNG/Debug.hpp>
#include <AVGNG/Scene.hpp>
#include <AVGNG/Cursor.hpp>

// Scripting
#include <AVGNG/LuaManager.hpp>

// Assets
#include <AVGNG/ObjFileParser.hpp>
#include <AVGNG/ShaderLoader.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define NG_DEVELOPER_MODE

#ifdef NG_DEVELOPER_MODE
	#include <AVGNG/InspectorView.hpp>	
#endif

namespace ng::Core {

		class Game {
			const char* windowTitle;
			glm::uvec2 windowSize;

		public:
			static ng::Core::Scene* activeScene;
			static ng::Graphics::Camera* camera;
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
