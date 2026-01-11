#include <AVGNG/KeyboardInput.hpp>

namespace ng::Core {

    std::unordered_map<int, bool> KeyboardInput::s_KeyDown;
    std::unordered_map<int, bool> KeyboardInput::s_KeyPressed;
    std::unordered_map<int, bool> KeyboardInput::s_KeyReleased;

	void KeyboardInput::Init(GLFWwindow* gameWindow)
	{
		// Prepare GLFW for keyboard input
		glfwSetInputMode(gameWindow, GLFW_STICKY_KEYS, GLFW_TRUE);
		glfwSetKeyCallback(gameWindow, KeyCallback);
	}

    void KeyboardInput::RegisterKeysWithLua(sol::state& lua) {
       
        lua["Key"] = lua.create_table();
        lua["Key"]["KEY_SPACE"] = GLFW_KEY_SPACE;  // or whatever your actual key code constants are
        lua["Key"]["KEY_W"] = GLFW_KEY_W;
        lua["Key"]["KEY_A"] = GLFW_KEY_A;
        lua["Key"]["KEY_S"] = GLFW_KEY_S;
        lua["Key"]["KEY_D"] = GLFW_KEY_D;
        lua["Key"]["KEY_LEFT_SHIFT"] = GLFW_KEY_LEFT_SHIFT;
        lua["Key"]["KEY_LEFT_CONTROL"] = GLFW_KEY_LEFT_CONTROL;

    }

    void KeyboardInput::RegisterKeyboardWithLua(sol::state& lua) {
            auto keyboardInput = lua.create_table();
            keyboardInput.set_function("IsKeyDown", &ng::Core::KeyboardInput::IsKeyDown);
            keyboardInput.set_function("IsKeyPressed", &ng::Core::KeyboardInput::IsKeyPressed);
            keyboardInput.set_function("IsKeyReleased", &ng::Core::KeyboardInput::IsKeyReleased);
            lua["KeyboardInput"] = keyboardInput;
    }

    void KeyboardInput::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        // Reset transient states
        s_KeyPressed[key] = false;
        s_KeyReleased[key] = false;

        if (action == GLFW_PRESS)
        {
            s_KeyDown[key] = true;
            s_KeyPressed[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            s_KeyDown[key] = false;
            s_KeyReleased[key] = true;
        }
        else if (action == GLFW_REPEAT)
        {
            s_KeyDown[key] = true;
        }
    }

    bool KeyboardInput::IsKeyDown(Key key)
    {
        return s_KeyDown[(int)key];
    }

    bool KeyboardInput::IsKeyPressed(Key key)
    {
        return s_KeyPressed[(int)key];
    }

    bool KeyboardInput::IsKeyReleased(Key key)
    {
        return s_KeyReleased[(int)key];
    }

}
