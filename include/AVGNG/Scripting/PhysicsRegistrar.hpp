#pragma once

namespace sol {
    class state;
}

namespace ng::Scripting {

// Registers physics and collision components for Sol2 Lua scripting
class PhysicsRegistrar {
private:
    static void RegisterCollider(sol::state& lua);
    static void RegisterPhysicsBody(sol::state& lua);

public:
    static void Register(sol::state& lua);
};

} // namespace ng::Scripting