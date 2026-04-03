#pragma once

#include <GLFW/glfw3.h>
#include <sol/sol.hpp>
#include <glm/vec2.hpp>

namespace ng::Core {

	class MouseInput {

	private:

		enum MouseButton {
			LEFT_BUTTON = GLFW_MOUSE_BUTTON_LEFT,
			RIGHT_BUTTON = GLFW_MOUSE_BUTTON_RIGHT,
			MIDDLE_BUTTON = GLFW_MOUSE_BUTTON_MIDDLE,
			BUTTON_4 = GLFW_MOUSE_BUTTON_4,
			BUTTON_5 = GLFW_MOUSE_BUTTON_5,
			BUTTON_6 = GLFW_MOUSE_BUTTON_6,
			BUTTON_7 = GLFW_MOUSE_BUTTON_7,
			BUTTON_8 = GLFW_MOUSE_BUTTON_8
		};

		static bool s_mouseButtonStates[8]; // glfw supports up to 8 mouse buttons

		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		

		static GLFWwindow* s_gameWindow;
		static void RegisterMouseButtonsWithLua(sol::state& lua);
	
	public:

		static glm::vec2 lastMousePosition;    // last mouse position
		static glm::vec2 currMousePosition;    // current mouse position

		static void Init(GLFWwindow& window);

		static glm::vec2 GetMousePosition(); // Returns current mouse position ( screen-space )
		static glm::vec2 GetMouseDelta();    // Returns change in mouse position since last frame
		static bool IsMouseButtonPressed(MouseButton button); // Returns true if mouse button is pressed

		static void RegisterMouseWithLua(sol::state& lua);

	};

}