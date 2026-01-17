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
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Inputs
#include <AVGNG/KeyboardInput.hpp>
#include <AVGNG/MouseInput.hpp>
#include <AVGNG/Cursor.hpp>

// Graphics
#include <AVGNG/Camera.hpp>
#include <AVGNG/Shader.hpp>
#include <AVGNG/Mesh.hpp>
#include <AVGNG/MeshRenderer.hpp>

// Core
#include <AVGNG/Time.hpp>
#include <AVGNG/Transform.hpp>
#include <AVGNG/GameObject.hpp>
#include <AVGNG/Scene.hpp>
#include <AVGNG/Debug.hpp>

// Scripting
#include <AVGNG/LuaManager.hpp>

// Assets
#include <AVGNG/ObjFileParser.hpp>
#include <AVGNG/ShaderLoader.hpp>


#ifdef NG_DEVELOPER_MODE
	// Editor UI
	#include <AVGNG/EditorUIElement.hpp>
	#include <AVGNG/EditorUI.hpp>

	// Editor UI Views
	#include <AVGNG/InspectorView.hpp>
	#include <AVGNG/HierarchyView.hpp>
	#include <AVGNG/ConsoleView.hpp>
#endif


namespace ng::Core {

		class Game {

		private:
			const char* windowTitle;
			glm::uvec2 defaultWindowSize;
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
