#pragma once
#include <sol/sol.hpp>

namespace ng::Scripting {

    // Registers generic lua commands for interacting with game interfaces
    class GraphicsRegistrar {
    public:
        static void Register(sol::state& lua); // 👈 Declaration only!
    };

}