#include <glad/glad.h> 
#include "AVGNG/Scripting/InputRegistrar.hpp"
// 🔌 Concrete input engine targets are safely linked here:
#include "AVGNG/Core/input/KeyboardInput.hpp"
#include "AVGNG/Core/input/MouseInput.hpp"
#include "AVGNG/Core/Cursor.hpp"

using namespace ng::Core;

namespace ng::Scripting {

    static void RegisterKeyboard(sol::state& lua) {
        auto keyboardInput = lua.create_table();
        keyboardInput.set_function("IsKeyDown", &ng::Core::KeyboardInput::IsKeyDown);
        keyboardInput.set_function("IsKeyPressed", &ng::Core::KeyboardInput::IsKeyPressed);
        keyboardInput.set_function("IsKeyReleased", &ng::Core::KeyboardInput::IsKeyReleased);
        lua["KeyboardInput"] = keyboardInput;

        KeyboardInput::RegisterKeysWithLua(lua);
    }

    static void RegisterMouse(sol::state& lua) {
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

    static void RegisterCursor(sol::state& lua) {
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

    void InputRegistrar::Register(sol::state& lua) {
        RegisterKeyboard(lua);
        RegisterMouse(lua);
        RegisterCursor(lua);
    }

}