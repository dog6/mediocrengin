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
#include "AVGNG/Core/input/KeyboardInput.hpp"
#include "AVGNG/Core/input/MouseInput.hpp"
#include "AVGNG/Core/Cursor.hpp"

// Graphics
#include "AVGNG/Graphics/Camera.hpp"
#include "AVGNG/Graphics/Shader.hpp"
#include "AVGNG/Graphics/Mesh.hpp"
#include "AVGNG/Graphics/MeshRenderer.hpp"

// Core
#include "AVGNG/Core/Scene.hpp"
#include "AVGNG/Core/SceneManager.hpp"
#include "AVGNG/Core/Time.hpp"
#include "AVGNG/Core/Transform.hpp"
#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/Debug.hpp"

// Collision (Core)
#include "AVGNG/Core/collision/AABB.hpp"
#include "AVGNG/Core/collision/BoundingBox.hpp"
#include "AVGNG/Core/collision/BoxShape.hpp"
#include "AVGNG/Core/collision/SphereShape.hpp"
#include "AVGNG/Core/collision/ColliderShape.hpp"
#include "AVGNG/Core/collision/Collider.hpp"
#include "AVGNG/Core/collision/CollisionSystem.hpp"


// Scripting

// Assets
#include "AVGNG/Graphics/ShaderLoader.hpp"


#ifdef NG_DEVELOPER_MODE
	// Editor UI
	#include "AVGNG/Editor/EditorUIElement.hpp"
	#include "AVGNG/Editor/EditorUI.hpp"

	// Editor UI Views
	#include "AVGNG/Editor/InspectorView.hpp"
	#include "AVGNG/Editor/HierarchyView.hpp"
	#include "AVGNG/Editor/ConsoleView.hpp"
#endif


namespace ng::Core {
	
	class Scene;

	class Game {

		private:
			const char* windowTitle;
			glm::uvec2 defaultWindowSize;
			glm::uvec2 viewportSize; // same as current window size, just unsigned int* instead
			glm::vec<2, int> currentWindowSize;

		public:

			static Game* Instance;
			static GLFWwindow* gameWindow;
			static ng::Core::SceneManager* sceneManager;
			static ng::Graphics::Camera* camera;

			Game();
			Game(const char* title, glm::uvec2 size);
			~Game();

			static ng::Core::SceneManager* GetSceneManager() { return sceneManager; }
			static ng::Core::Scene* GetActiveScene() { return sceneManager->GetActiveScene(); } // pass through function for lua scripting
			
			// Getters required for lua bindings
			static ng::Core::Game* GetInstance() { return Game::Instance; }

			// viewportSize is non-static; scenes keep a reference to it so they track window resizes
			static glm::uvec2& GetViewportSize() { return Instance->viewportSize; }

			void Init();    // Called when game first starts up
			void Load();    // Called before game loop starts
			void Start();    // Called when game first starts
			void Run();    // Called after Start(), GameLoop
			void Exit();    // Called when game is closed

			static glm::uvec2 GetWindowSize();

		};

}
