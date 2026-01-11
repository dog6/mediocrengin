#pragma once

#include <sol/sol.hpp>

namespace ng::Core {
	class Scene;
}

namespace ng::Scripting {

	class LuaManager {

	private:
		static void BindToLua(sol::state& lua);
		static sol::state m_lua;

	public:
		static void Init(ng::Core::Scene* scene);
		static sol::state& GetState();

		static void Update(float deltaTime);
		static void Load(ng::Core::Scene* scene, const char* scriptPath);

	};

}