
#include <AVGNG/Scene.hpp>
#include <AVGNG/GameObject.hpp>
#include <AVGNG/Transform.hpp>
#include <AVGNG/MeshRenderer.hpp>
#include <AVGNG/LuaManager.hpp>
#include <AVGNG/ObjFileParser.hpp>

using namespace ng::Core;
using namespace ng::Graphics;

namespace ng::Scripting {

	sol::state LuaManager::m_lua;

	// Helper Methods
	void RegisterScene(sol::state& lua) {
		lua.new_usertype<ng::Core::Scene>("Scene",
			"CreateGameObject", &ng::Core::Scene::CreateGameObject,
			// Meta-functions satisfy the compiler's need for comparison operators
			sol::meta_function::equal_to, [](const ng::Core::Scene& a, const ng::Core::Scene& b) { return &a == &b; },
			sol::meta_function::less_than, [](const ng::Core::Scene& a, const ng::Core::Scene& b) { return &a < &b; }
		);
	}

	void RegisterGameObject(sol::state& lua) {
		lua.new_usertype<ng::Core::GameObject>("GameObject",
			"name", &ng::Core::GameObject::name,
			"AddComponent", &ng::Core::GameObject::AddComponentByName,

			// Add this wrapper for GetComponent
			"GetComponent", [](ng::Core::GameObject& self, std::string name, sol::this_state s) -> sol::object {
				auto* comp = self.GetComponentByName(name);
				if (!comp) return sol::nil;

				// Ensure Lua knows it's a Transform so we can call SetPosition
				if (name == "Transform") {
					return sol::make_object(s, static_cast<ng::Core::Transform*>(comp));
				}
				else if (name == "MeshRenderer") {
					return sol::make_object(s, static_cast<ng::Graphics::MeshRenderer*>(comp));
				}

				return sol::make_object(s, comp);
			}
		);
	}

	void RegisterTransform(sol::state& lua) {
		lua.new_usertype<ng::Core::Transform>("Transform",
			sol::base_classes, sol::bases<ng::Core::Component>(),
			"SetPosition", [](ng::Core::Transform& self, float x, float y, float z) {
				self.SetPosition(glm::vec3(x, y, z));
			},
			"SetRotation", [](ng::Core::Transform& self, float x, float y, float z) {
				self.SetRotation(glm::vec3(x, y, z));
			},
			"SetScale", [](ng::Core::Transform& self, float x, float y, float z) {
				self.SetScale(glm::vec3(x, y, z));
			}
		);
	}

	void RegisterMeshRenderer(sol::state& lua) {
		lua.new_usertype<ng::Graphics::MeshRenderer>("MeshRenderer",
			sol::base_classes, sol::bases<ng::Core::Component>(),
			"LoadMesh", &ng::Graphics::MeshRenderer::LoadMesh,
			"LoadShader", &ng::Graphics::MeshRenderer::LoadShader
		);
	}

	// LuaManager Methods
	void LuaManager::BindToLua(sol::state& lua) {
		
		Debug::Log(LOG, "Binding to LuaVM");

		lua.new_usertype<ng::Core::Component>("Component");

		RegisterScene(lua);
		RegisterGameObject(lua);
		RegisterTransform(lua);
		RegisterMeshRenderer(lua);

	}

	void LuaManager::Init(ng::Core::Scene* scene)
	{
		m_lua.open_libraries(
			sol::lib::base,
			sol::lib::package,
			sol::lib::math,
			sol::lib::table,
			sol::lib::io
		);

		if (m_lua["print"].valid()) {
			Debug::Log(LOG, "C++ Verify: 'print' is valid in Lua state.");
		}
		else {
			Debug::Log(ERROR, "C++ Verify: 'print' is STILL NULL after open_libraries!");
		}

		BindToLua(m_lua);
		m_lua["scene"] = scene;
	}

	sol::state& LuaManager::GetState() {
		return m_lua;
	}

	void LuaManager::Load(Scene* scene, const char* scriptPath) {
		if (std::filesystem::exists(scriptPath)) {
			// Make scene available to Lua BEFORE running the script
			m_lua["scene"] = scene;

			try {
				auto result = m_lua.script_file(scriptPath);

				// Check if script executed successfully
				if (!result.valid()) {
					sol::error err = result;
					Debug::Log(ERROR, "Lua script error in '%s': %s", scriptPath, err.what());
				}
				Debug::Log(LOG, "Lua script executed successfully! '%s'", scriptPath);
			}
			catch (const sol::error& e) {
				std::cout << "Lua Error: " << e.what() << std::endl;
			}
			

		}
		else {
			Debug::Log(ERROR, "Failed to find lua script: '%s'.", scriptPath);
		}
	}

	void LuaManager::Update(float deltaTime) {
		// Look up the function in the global table
		sol::protected_function updateFunc = m_lua["OnUpdate"];

		if (updateFunc.valid()) {
			auto result = updateFunc(deltaTime);
			if (!result.valid()) {
				sol::error err = result;
				Debug::Log(ERROR, "Lua Update Error: %s", err.what());
			}
		}
	}


}