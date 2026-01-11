#pragma once

#include <sol/sol.hpp>

namespace ng::Core {
	class Scene;
}

namespace ng::Scripting {

	class LuaManager {

	public:
		struct LuaScript {
			sol::state lua;
			std::string filepath;
		};

		static void Load(ng::Core::Scene* scene, const char* scriptPath);
		static void Update(float deltaTime);
		static void BindToLua(sol::state& lua);
		static void Cleanup();

	private:

		static std::vector<LuaScript> s_scripts;


	};

}