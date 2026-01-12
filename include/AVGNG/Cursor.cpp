#include "Cursor.hpp"


namespace ng::Core {

	GLFWwindow* Cursor::s_gameWindow;
	CursorLockState Cursor::s_lockState;

	void Cursor::Init(GLFWwindow* gameWindow)
	{
		Cursor::s_gameWindow = gameWindow;
	}

	void Cursor::OnCursorLockModeChanged()
	{

		switch(s_lockState.lockMode){
		
		case CursorLockMode::CONFINED:
			if (s_lockState.isVisible) { glfwSetInputMode(s_gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }
			else { glfwSetInputMode(s_gameWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE); }
			break;
		case CursorLockMode::LOCKED:
			
			if (s_lockState.isVisible) { glfwSetInputMode(s_gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }
			else { glfwSetInputMode(s_gameWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE); }
				
				break;
		default:

			// NONE
			glfwSetInputMode(s_gameWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			break;
		
		}

		Debug::Log(LOG, "Cursor lock mode changed to %d", (int)s_lockState.lockMode);


	}

	void Cursor::RegisterCursorWithLua(sol::state& lua)
	{
		// Cursor lock modes
		lua["CursorLockMode"] = lua.create_table();
		lua["CursorLockMode"]["NONE"] = CursorLockMode::NONE;
		lua["CursorLockMode"]["LOCKED"] = CursorLockMode::LOCKED;
		lua["CursorLockMode"]["CONFINED"] = CursorLockMode::CONFINED;

		auto cursor = lua.create_table();

		cursor.set_function("SetCursorLockMode", [](int mode) {
			ng::Core::Cursor::SetCursorLockMode(static_cast<CursorLockMode>(mode));
		});

		lua["Cursor"] = cursor;

	}

}