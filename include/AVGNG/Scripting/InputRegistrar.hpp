#pragma once
#include <sol/sol.hpp>

namespace ng::Scripting {

    // Registers generic lua commands for interacting with engine input subsystems
    class InputRegistrar {
    public:
        static void Register(sol::state& lua); // 👈 Declaration only!
    };

}