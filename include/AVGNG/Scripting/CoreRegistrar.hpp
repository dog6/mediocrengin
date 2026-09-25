#pragma once
#include <sol/sol.hpp>

namespace ng::Scripting {

    // Registers generic lua commands for interacting with core engine interfaces
    class CoreRegistrar {
    public:
        static void Register(sol::state& lua);
    };

}