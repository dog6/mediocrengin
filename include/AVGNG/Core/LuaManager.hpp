#pragma once

#include <sol/sol.hpp>
#include <string>

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

		static void LoadNoclip(); // should be moved eventually
	};

}