#include "AVGNG/Core/Cursor.hpp"
#include "AVGNG/Core/Debug.hpp"

namespace ng::Core {

	GLFWwindow* Cursor::s_gameWindow;
	CursorLockState Cursor::s_lockState;

    void Cursor::SetCursorLockMode(CursorLockMode mode)
    {
		Debug::Log(DEBUG, "Set cursor lock mode to: %d", (int)mode);
		s_lockState.lockMode = mode;
		OnCursorLockModeChanged();
    }

    void Cursor::Init(GLFWwindow *gameWindow)
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

	

}