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

        // Letters
        lua["Key"]["KEY_A"] = Key::KEY_A;
        lua["Key"]["KEY_B"] = Key::KEY_B;
        lua["Key"]["KEY_C"] = Key::KEY_C;
        lua["Key"]["KEY_D"] = Key::KEY_D;
        lua["Key"]["KEY_E"] = Key::KEY_E;
        lua["Key"]["KEY_F"] = Key::KEY_F;
        lua["Key"]["KEY_G"] = Key::KEY_G;
        lua["Key"]["KEY_H"] = Key::KEY_H;
        lua["Key"]["KEY_I"] = Key::KEY_I;
        lua["Key"]["KEY_J"] = Key::KEY_J;
        lua["Key"]["KEY_K"] = Key::KEY_K;
        lua["Key"]["KEY_L"] = Key::KEY_L;
        lua["Key"]["KEY_M"] = Key::KEY_M;
        lua["Key"]["KEY_N"] = Key::KEY_N;
        lua["Key"]["KEY_O"] = Key::KEY_O;
        lua["Key"]["KEY_P"] = Key::KEY_P;
        lua["Key"]["KEY_Q"] = Key::KEY_Q;
        lua["Key"]["KEY_R"] = Key::KEY_R;
        lua["Key"]["KEY_S"] = Key::KEY_S;
        lua["Key"]["KEY_T"] = Key::KEY_T;
        lua["Key"]["KEY_U"] = Key::KEY_U;
        lua["Key"]["KEY_V"] = Key::KEY_V;
        lua["Key"]["KEY_W"] = Key::KEY_W;
        lua["Key"]["KEY_X"] = Key::KEY_X;
        lua["Key"]["KEY_Y"] = Key::KEY_Y;
        lua["Key"]["KEY_Z"] = Key::KEY_Z;

        // Numbers (top row)
        lua["Key"]["KEY_0"] = Key::KEY_0;
        lua["Key"]["KEY_1"] = Key::KEY_1;
        lua["Key"]["KEY_2"] = Key::KEY_2;
        lua["Key"]["KEY_3"] = Key::KEY_3;
        lua["Key"]["KEY_4"] = Key::KEY_4;
        lua["Key"]["KEY_5"] = Key::KEY_5;
        lua["Key"]["KEY_6"] = Key::KEY_6;
        lua["Key"]["KEY_7"] = Key::KEY_7;
        lua["Key"]["KEY_8"] = Key::KEY_8;
        lua["Key"]["KEY_9"] = Key::KEY_9;

        // Function keys
        lua["Key"]["KEY_F1"] = Key::KEY_F1;
        lua["Key"]["KEY_F2"] = Key::KEY_F2;
        lua["Key"]["KEY_F3"] = Key::KEY_F3;
        lua["Key"]["KEY_F4"] = Key::KEY_F4;
        lua["Key"]["KEY_F5"] = Key::KEY_F5;
        lua["Key"]["KEY_F6"] = Key::KEY_F6;
        lua["Key"]["KEY_F7"] = Key::KEY_F7;
        lua["Key"]["KEY_F8"] = Key::KEY_F8;
        lua["Key"]["KEY_F9"] = Key::KEY_F9;
        lua["Key"]["KEY_F10"] = Key::KEY_F10;
        lua["Key"]["KEY_F11"] = Key::KEY_F11;
        lua["Key"]["KEY_F12"] = Key::KEY_F12;

        // Control keys
        lua["Key"]["KEY_ESCAPE"] = Key::KEY_ESCAPE;
        lua["Key"]["KEY_ENTER"] = Key::KEY_ENTER;
        lua["Key"]["KEY_TAB"] = Key::KEY_TAB;
        lua["Key"]["KEY_BACKSPACE"] = Key::KEY_BACKSPACE;
        lua["Key"]["KEY_INSERT"] = Key::KEY_INSERT;
        lua["Key"]["KEY_DELETE"] = Key::KEY_DELETE;
        lua["Key"]["KEY_RIGHT"] = Key::KEY_RIGHT;
        lua["Key"]["KEY_LEFT"] = Key::KEY_LEFT;
        lua["Key"]["KEY_DOWN"] = Key::KEY_DOWN;
        lua["Key"]["KEY_UP"] = Key::KEY_UP;
        lua["Key"]["KEY_PAGE_UP"] = Key::KEY_PAGE_UP;
        lua["Key"]["KEY_PAGE_DOWN"] = Key::KEY_PAGE_DOWN;
        lua["Key"]["KEY_HOME"] = Key::KEY_HOME;
        lua["Key"]["KEY_END"] = Key::KEY_END;
        lua["Key"]["KEY_CAPS_LOCK"] = Key::KEY_CAPS_LOCK;
        lua["Key"]["KEY_SCROLL_LOCK"] = Key::KEY_SCROLL_LOCK;
        lua["Key"]["KEY_NUM_LOCK"] = Key::KEY_NUM_LOCK;
        lua["Key"]["KEY_PRINT_SCREEN"] = Key::KEY_PRINT_SCREEN;
        lua["Key"]["KEY_PAUSE"] = Key::KEY_PAUSE;

        // Modifier keys
        lua["Key"]["KEY_LEFT_SHIFT"] = Key::KEY_LEFT_SHIFT;
        lua["Key"]["KEY_LEFT_CONTROL"] = Key::KEY_LEFT_CONTROL;
        lua["Key"]["KEY_LEFT_ALT"] = Key::KEY_LEFT_ALT;
        lua["Key"]["KEY_LEFT_SUPER"] = Key::KEY_LEFT_SUPER;
        lua["Key"]["KEY_RIGHT_SHIFT"] = Key::KEY_RIGHT_SHIFT;
        lua["Key"]["KEY_RIGHT_CONTROL"] = Key::KEY_RIGHT_CONTROL;
        lua["Key"]["KEY_RIGHT_ALT"] = Key::KEY_RIGHT_ALT;
        lua["Key"]["KEY_RIGHT_SUPER"] = Key::KEY_RIGHT_SUPER;
        lua["Key"]["KEY_MENU"] = Key::KEY_MENU;

        // Symbols
        lua["Key"]["KEY_SPACE"] = Key::KEY_SPACE;
        lua["Key"]["KEY_APOSTROPHE"] = Key::KEY_APOSTROPHE;
        lua["Key"]["KEY_COMMA"] = Key::KEY_COMMA;
        lua["Key"]["KEY_MINUS"] = Key::KEY_MINUS;
        lua["Key"]["KEY_PERIOD"] = Key::KEY_PERIOD;
        lua["Key"]["KEY_SLASH"] = Key::KEY_SLASH;
        lua["Key"]["KEY_SEMICOLON"] = Key::KEY_SEMICOLON;
        lua["Key"]["KEY_EQUAL"] = Key::KEY_EQUAL;
        lua["Key"]["KEY_LEFT_BRACKET"] = Key::KEY_LEFT_BRACKET;
        lua["Key"]["KEY_BACKSLASH"] = Key::KEY_BACKSLASH;
        lua["Key"]["KEY_RIGHT_BRACKET"] = Key::KEY_RIGHT_BRACKET;
        lua["Key"]["KEY_GRAVE_ACCENT"] = Key::KEY_GRAVE_ACCENT;

        // Keypad
        lua["Key"]["KEY_KP_0"] = Key::KEY_KP_0;
        lua["Key"]["KEY_KP_1"] = Key::KEY_KP_1;
        lua["Key"]["KEY_KP_2"] = Key::KEY_KP_2;
        lua["Key"]["KEY_KP_3"] = Key::KEY_KP_3;
        lua["Key"]["KEY_KP_4"] = Key::KEY_KP_4;
        lua["Key"]["KEY_KP_5"] = Key::KEY_KP_5;
        lua["Key"]["KEY_KP_6"] = Key::KEY_KP_6;
        lua["Key"]["KEY_KP_7"] = Key::KEY_KP_7;
        lua["Key"]["KEY_KP_8"] = Key::KEY_KP_8;
        lua["Key"]["KEY_KP_9"] = Key::KEY_KP_9;
        lua["Key"]["KEY_KP_DECIMAL"] = Key::KEY_KP_DECIMAL;
        lua["Key"]["KEY_KP_DIVIDE"] = Key::KEY_KP_DIVIDE;
        lua["Key"]["KEY_KP_MULTIPLY"] = Key::KEY_KP_MULTIPLY;
        lua["Key"]["KEY_KP_SUBTRACT"] = Key::KEY_KP_SUBTRACT;
        lua["Key"]["KEY_KP_ADD"] = Key::KEY_KP_ADD;
        lua["Key"]["KEY_KP_ENTER"] = Key::KEY_KP_ENTER;
        lua["Key"]["KEY_KP_EQUAL"] = Key::KEY_KP_EQUAL;
    }

    void KeyboardInput::RegisterKeyboardWithLua(sol::state& lua) {
            auto keyboardInput = lua.create_table();
            keyboardInput.set_function("IsKeyDown", &ng::Core::KeyboardInput::IsKeyDown);
            keyboardInput.set_function("IsKeyPressed", &ng::Core::KeyboardInput::IsKeyPressed);
            keyboardInput.set_function("IsKeyReleased", &ng::Core::KeyboardInput::IsKeyReleased);
            lua["KeyboardInput"] = keyboardInput;

            RegisterKeysWithLua(lua);

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
        bool result = s_KeyPressed[(int)key];
        s_KeyPressed[(int)key] = false;
        return result;
    }

    bool KeyboardInput::IsKeyReleased(Key key)
    {
        return s_KeyReleased[(int)key];
    }

}
