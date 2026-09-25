#pragma once

#include <sol/sol.hpp>
#include <string>
#include "AVGNG/Core/Game.hpp"
#include "AVGNG/Assets/FileReader.hpp"

#include "AVGNG/Scripting/CoreRegistrar.hpp"
#include "AVGNG/Scripting/GraphicsRegistrar.hpp"
#include "AVGNG/Scripting/PhysicsRegistrar.hpp"
#include "AVGNG/Scripting/InputRegistrar.hpp"


#ifdef NG_DEVELOPER_MODE
#include "AVGNG/Editor/ConsoleView.hpp"
#endif

namespace ng::Core {
	class Scene;
}

namespace ng::Scripting {

	struct LuaScript {
		sol::state lua;
		std::string filepath;
	};

	class LuaManager {

	private:

		static std::vector<LuaScript> s_scripts;

	public:

		static void Load(const char* scriptPath);
		static void Update(float deltaTime);
		static void BindToLua(sol::state& lua);
		static void Cleanup();
		static void Execute(std::string luaCode);

	};

}