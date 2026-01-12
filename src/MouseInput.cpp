#include <AVGNG/MouseInput.hpp>

namespace ng::Core {

	GLFWwindow* MouseInput::s_gameWindow;
	glm::vec2 MouseInput::lastMousePosition;    // last mouse position
	glm::vec2 MouseInput::currMousePosition;    // current mouse position

	bool MouseInput::s_mouseButtonStates[8] = { false };

	void MouseInput::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{

		if (action == GLFW_PRESS)
		{
			s_mouseButtonStates[button] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			s_mouseButtonStates[button] = false;
		}

	}

	void MouseInput::Init(GLFWwindow& gameWindow)
	{
		s_gameWindow = &gameWindow;
		glfwSetMouseButtonCallback(&gameWindow, MouseButtonCallback);
	}

	glm::vec2 MouseInput::GetMousePosition() {

		lastMousePosition = currMousePosition;

		double* x = new double();
		double* y = new double();
		glfwGetCursorPos(s_gameWindow, x, y);

		currMousePosition = glm::vec2((float)*x, (float)*y);

		return currMousePosition;
	}

	glm::vec2 MouseInput::GetMouseDelta() {
		GetMousePosition(); // ensure we have a lastMousePosition captured
		return currMousePosition - lastMousePosition;
	}

	bool MouseInput::IsMouseButtonPressed(MouseButton button)
	{
		return s_mouseButtonStates[(int)button];
	}

	void MouseInput::RegisterMouseWithLua(sol::state& lua)
	{

		auto mouseInput = lua.create_table();

		mouseInput.set_function("GetMousePosition", []() {
			glm::vec2 pos = ng::Core::MouseInput::GetMousePosition();
			return std::make_tuple(pos.x, pos.y);
		});

		mouseInput.set_function("GetMouseDelta", []() {
			glm::vec2 delta = ng::Core::MouseInput::GetMouseDelta();
			return std::make_tuple(delta.x, delta.y);
		});

		mouseInput.set_function("IsMouseButtonPressed", &ng::Core::MouseInput::IsMouseButtonPressed);

		lua["MouseInput"] = mouseInput;

	}

	void MouseInput::RegisterMouseButtonsWithLua(sol::state& lua)
	{
		lua["MouseButton"] = lua.create_table();
		lua["MouseButton"]["LEFT_BUTTON"] = MouseButton::LEFT_BUTTON;
		lua["MouseButton"]["RIGHT_BUTTON"] = MouseButton::RIGHT_BUTTON;
		lua["MouseButton"]["MIDDLE_BUTTON"] = MouseButton::MIDDLE_BUTTON;
		lua["MouseButton"]["BUTTON_4"] = MouseButton::BUTTON_4;
		lua["MouseButton"]["BUTTON_5"] = MouseButton::BUTTON_5;
		lua["MouseButton"]["BUTTON_6"] = MouseButton::BUTTON_6;
		lua["MouseButton"]["BUTTON_7"] = MouseButton::BUTTON_7;
		lua["MouseButton"]["BUTTON_8"] = MouseButton::BUTTON_8;
	}

}