
#include <AVGNG/LuaManager.hpp>


#include <AVGNG/Scene.hpp>
#include <AVGNG/GameObject.hpp>
#include <AVGNG/Transform.hpp>
#include <AVGNG/MeshRenderer.hpp>
#include <AVGNG/ObjFileParser.hpp>

#include <AVGNG/KeyboardInput.hpp>
#include <AVGNG/MouseInput.hpp>

#include <AVGNG/Cursor.hpp>

using namespace ng::Core;
using namespace ng::Graphics;

namespace ng::Scripting {

		//// Helper Methods
		//// Register Lua Bindings
		////
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
		  		},
		  		"GetPosition", [](ng::Core::Transform& self) { self.GetPosition(); },
		  		"GetRotation", [](ng::Core::Transform& self) { self.GetRotation(); },
		  		"GetScale", [](ng::Core::Transform& self) { self.GetScale(); }
			);
		}
		  
		void RegisterCamera(sol::state& lua) {

			lua.new_usertype<ng::Graphics::Camera>("Camera",
				"SetPosition", [](ng::Graphics::Camera& cam, float x, float y, float z) {
					cam.SetPosition(glm::vec3(x, y, z));
				},
				"GetPosition", [](ng::Graphics::Camera& cam) {
					glm::vec3 camPos = cam.GetPosition();
					return std::make_tuple(camPos.x, camPos.y, camPos.z);
				},
				"SetTarget", [](ng::Graphics::Camera& cam, float x, float y, float z) {
					cam.SetTarget(glm::vec3(x, y, z));
				},
				"GetTarget", [](ng::Graphics::Camera& cam) {
					glm::vec3 camTarget = cam.GetTarget();
					return std::make_tuple(camTarget.x, camTarget.y, camTarget.z);
				}, 
				"SetUpwardDirection", [](ng::Graphics::Camera& cam, float x, float y, float z) {
					cam.SetUp(glm::vec3(x, y, z));
				},
				"GetUpwardDirection", [](ng::Graphics::Camera& cam) {
					glm::vec3 camUp = cam.GetUp();
					return std::make_tuple(camUp.x, camUp.y, camUp.z);
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
		
		std::vector<LuaManager::LuaScript> LuaManager::s_scripts;

		/// <summary>
		/// Loads a script file and binds it to the active scene.
		/// </summary>
		/// <param name="scene">Scene* reference</param>
		/// <param name="filepath">Path to lua script</param>
		void LuaManager::Load(ng::Core::Scene* scene, const char* filepath) {
			
			Debug::Log(LOG, "Loading Lua script: '%s'", filepath);

			LuaScript script;
			script.filepath = filepath;

			script.lua.open_libraries(
				sol::lib::base,
				sol::lib::package,
				sol::lib::math,
				sol::lib::table,
				sol::lib::string
			);

			BindToLua(script.lua);

			script.lua["scene"] = scene;
			script.lua["camera"] = scene->GetActiveCamera();

			try {
				auto result = script.lua.script_file(filepath);
				if (!result.valid()) {
					sol::error err = result;
					Debug::Log(ERROR, "Lua script error in '%s': %s", filepath, err.what());
					return;
				}
			}
			catch (const sol::error& e) {
				Debug::Log(ERROR, "Exception loading Lua script '%s': %s", filepath, e.what());
				return;
			}
			

			s_scripts.push_back(std::move(script));

			Debug::Log(LOG, " -> Loaded Lua script: '%s'\n\n", filepath);

		}

		/// <summary>
		/// Calls OnUpdate function in all loaded scripts.
		/// </summary>
		/// <param name="deltaTime">Time.deltaTime</param>
		void LuaManager::Update(float deltaTime) {
			
			for (auto& script : s_scripts) {

				sol::protected_function updateFunc = script.lua["OnUpdate"];

				if (updateFunc.valid()) {

					auto result = updateFunc(deltaTime);
					if (!result.valid()) {
						sol::error err = result;
						Debug::Log(ERROR, "Lua Update Error in %s: %s",
							script.filepath.c_str(), err.what());
					}

				}

			}

		}

		/// <summary>
		/// Binds passed lua script state to engine components.
		/// </summary>
		/// <param name="lua">sol::state& reference</param>
		void LuaManager::BindToLua(sol::state& lua) {
		
			RegisterScene(lua);
			RegisterGameObject(lua);
			RegisterTransform(lua);
			RegisterMeshRenderer(lua);
			RegisterCamera(lua);
			
			KeyboardInput::RegisterKeyboardWithLua(lua);
			MouseInput::RegisterMouseWithLua(lua);

			Cursor::RegisterCursorWithLua(lua);

		}

		/// <summary>
		/// Unloads all loaded lua scripts.
		/// </summary>
		void LuaManager::Cleanup()
		{
			s_scripts.clear();
		}

}