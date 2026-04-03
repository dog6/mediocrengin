#pragma once


// =====================================================================================================
// AVGNG Engine
// ----------------------------------
// 
// Developer mode can be enabled by adding NG_DEVELOPER_MODE to your preprocessor definitions.
// Simply defining it here won't work as other files may have already been compiled.
// 
// Debug mode can be enabled by adding NG_DEBUG_MODE to your preprocessor definitions.
// 
// In VSStudio, right-click project -> properties -> C/C++ -> Preprocessor -> Preprocessor Definitions
// and add them there.
// =====================================================================================================
#define STB_IMAGE_IMPLEMENTATION

// std includes
#include <cstdio>
#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <sstream>

// external libs
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// Inputs
#include "AVGNG/Core/KeyboardInput.hpp"
#include "AVGNG/Core/MouseInput.hpp"
#include "AVGNG/UI/Cursor.hpp"

// Graphics
#include "AVGNG/Renderer/Camera.hpp"
#include "AVGNG/Renderer/Shader.hpp"
#include "AVGNG/Renderer/Mesh.hpp"
#include "AVGNG/Renderer/MeshRenderer.hpp"

// Core
#include "AVGNG/Core/Time.hpp"
#include "AVGNG/Core/Transform.hpp"
#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/Scene.hpp"
#include "AVGNG/Utilities/Debug.hpp"

// Scripting
#include "AVGNG/Core/LuaManager.hpp"

// Assets
#include "AVGNG/Renderer/ShaderLoader.hpp"


#ifdef NG_DEVELOPER_MODE
	// Editor UI
	#include "AVGNG/UI/EditorUIElement.hpp"
	#include "AVGNG/UI/EditorUI.hpp"

	// Editor UI Views
	#include "AVGNG/UI/InspectorView.hpp"
	#include "AVGNG/UI/HierarchyView.hpp"
	#include "AVGNG/UI/ConsoleView.hpp"
#endif


namespace ng::Core {

		class Game {

		private:
			const char* windowTitle;
			glm::uvec2 defaultWindowSize;
			glm::uvec2 viewportSize; // same as current window size, just unsigned int* instead
			glm::vec<2, int> currentWindowSize;

		public:

			static Game* Instance;
			static GLFWwindow* gameWindow;
			static ng::Core::Scene* activeScene;
			static ng::Graphics::Camera* camera;

			Game();
			Game(const char* title, glm::uvec2 size);
			~Game();

			// Getters required for lua bindings
			static ng::Core::Scene* GetActiveScene() { return activeScene; }
			static ng::Core::Game* GetInstance() { return Game::Instance; }

			void Init();    // Called when game first starts up
			void Load();    // Called before game loop starts
			void Start();    // Called when game first starts
			void Run();    // Called after Start(), GameLoop
			void Exit();    // Called when game is closed

			static glm::uvec2 GetWindowSize();

		};

}
