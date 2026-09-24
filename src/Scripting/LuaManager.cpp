
#include "AVGNG/Scripting/LuaManager.hpp"

using namespace ng::Core;
using namespace ng::Graphics;

namespace ng::Scripting {

		//// Helper Methods
		//// Register Lua Bindings
		////
		
		void RegisterGame(sol::state& lua) {
			lua.new_usertype<Game>("Game",
				"GetActiveScene", [](Game& game) {
            		return game.GetSceneManager()->GetActiveScene();
        		}
			);
		}
	
		void RegisterScene(sol::state& lua) {
			lua.new_usertype<Scene>("Scene",
				"CreateGameObject", &Scene::CreateGameObject,
				// Meta-functions satisfy the compiler's need for comparison operators
				sol::meta_function::equal_to, [](const Scene& a, const Scene& b) { return &a == &b; },
				sol::meta_function::less_than, [](const Scene& a, const Scene& b) { return &a < &b; }
			);
		}
	
		void RegisterGameObject(sol::state& lua) {
			lua.new_usertype<GameObject>("GameObject",
				"name", &GameObject::name,
				"AddComponent", &GameObject::AddComponentByName,
	
				// Add this wrapper for GetComponent
				"GetComponent", [](GameObject& self, std::string name, sol::this_state s) -> sol::object {
					auto* comp = self.GetComponentByName(name);
					if (!comp) return sol::nil;
	
					// Ensure Lua knows it's a Transform so we can call SetPosition
					if (name == "Transform") {
						return sol::make_object(s, static_cast<Transform*>(comp));
					}
					else if (name == "MeshRenderer") {
						return sol::make_object(s, static_cast<MeshRenderer*>(comp));
					}
					else if (name == "PhysicsBody"){
						return sol::make_object(s, static_cast<PhysicsBody*>(comp));
					}
	
					return sol::make_object(s, comp);
				}
			);
		}
		
		void RegisterTransform(sol::state& lua) {
			lua.new_usertype<Transform>("Transform",
		  		sol::base_classes, sol::bases<IComponent>(),
		  		"SetPosition", [](Transform& self, float x, float y, float z) {
		  			self.SetPosition(glm::vec3(x, y, z));
		  		},
		  		"SetRotation", [](Transform& self, float x, float y, float z) {
		  			self.SetRotation(glm::vec3(x, y, z));
		  		},
		  		"SetScale", [](Transform& self, float x, float y, float z) {
		  			self.SetScale(glm::vec3(x, y, z));
		  		},
		  		"GetPosition", [](Transform& self) { self.GetPosition(); },
		  		"GetRotation", [](Transform& self) { self.GetRotation(); },
		  		"GetScale", [](Transform& self) { self.GetScale(); }
			);
		}
		  
		void RegisterCamera(sol::state& lua) {

			lua.new_usertype<Camera>("Camera",
				"SetPosition", [](Camera& cam, float x, float y, float z) {
					cam.SetPosition(glm::vec3(x, y, z));
				},
				"GetPosition", [](Camera& cam) {
					glm::vec3 camPos = cam.GetPosition();
					return std::make_tuple(camPos.x, camPos.y, camPos.z);
				},
				"SetTarget", [](Camera& cam, float x, float y, float z) {
					cam.SetTarget(glm::vec3(x, y, z));
				},
				"GetTarget", [](Camera& cam) {
					glm::vec3 camTarget = cam.GetTarget();
					return std::make_tuple(camTarget.x, camTarget.y, camTarget.z);
				}, 
				"SetUpwardDirection", [](Camera& cam, float x, float y, float z) {
					cam.SetUp(glm::vec3(x, y, z));
				},
				"GetUpwardDirection", [](Camera& cam) {
					glm::vec3 camUp = cam.GetUp();
					return std::make_tuple(camUp.x, camUp.y, camUp.z);
				}
			);
		  
		}


		void RegisterSphereCollider(sol::state& lua){
			lua.new_usertype<SphereCollider>("SphereCollider", 
				"SetRadius", [](SphereCollider& self, float r) {
		  			self.SetRadius(r);
		  		}
			);
		}

		
		void RegisterMesh(sol::state& lua) {
			lua.new_usertype<Mesh>("Mesh",
				"filepath", &Mesh::filepath,
				"material", &Mesh::GetMaterial
			);
		}

		void RegisterMaterial(sol::state& lua) {
			lua.new_usertype<Material>("Material",
				"SetShader", &Material::SetShader,
				"GetShader", &Material::GetShader,
				"SetMaterialData", &Material::SetMaterialData,
				"GetMaterialData", &Material::GetMaterialData
			);
		}

		void RegisterMeshRenderer(sol::state& lua) {
			lua.new_usertype<MeshRenderer>("MeshRenderer",
		  		sol::base_classes, sol::bases<IComponent>(),
		  		"LoadMesh", &MeshRenderer::LoadMesh,
				"GetMesh", &MeshRenderer::GetMesh
			);
		}
		
		void RegisterPhysicsBody(sol::state& lua) {
			lua.new_usertype<PhysicsBody>("PhysicsBody",
				sol::base_classes, sol::bases<IComponent>(),

				"SetLinearAcceleration", [](PhysicsBody& self, float x, float y, float z) {
					self.SetLinearAcceleration(glm::vec3(x, y, z));
				},

				"SetLinearVelocity", [](PhysicsBody& self, float x, float y, float z) {
					self.SetLinearVelocity(glm::vec3(x, y, z));
				},

				"SetAngularAcceleration", [](PhysicsBody& self, float x, float y, float z) {
					self.SetAngularAcceleration(glm::vec3(x, y, z));
				},

				"SetAngularVelocity", [](PhysicsBody& self, float x, float y, float z) {
					self.SetAngularVelocity(glm::vec3(x, y, z));
				}
			);
		}

		void RegisterShaderLoader(sol::state& lua) {
			lua.new_usertype<ng::Assets::ShaderLoader>("ShaderLoader",
				"LoadShaderFromFile", &ng::Assets::ShaderLoader::LoadShader,
				"LoadDefaultShader", &ng::Assets::ShaderLoader::LoadDefaultShader
			);
		}

		void RegisterDeveloperConsole(sol::state& lua) {
			// override lua print function
			lua["print"] = [](sol::variadic_args args) {
				bool first = true;
				for (auto arg : args) {
					if (!first) std::cout << "\t"; // separate with tab like Lua
					first = false;

					// Convert argument to string based on type
					if (arg.is<std::string>()) {
						std::cout << arg.as<std::string>();
					}
					else if (arg.is<bool>()) {
						std::cout << (arg.as<bool>() ? "true" : "false");
					}
					else if (arg.is<double>()) {
						std::cout << arg.as<double>();
					}
					else if (arg.is<int>()) {
						std::cout << arg.as<int>();
					}
					else {
						std::cout << "userdata"; // fallback for other types
					}
				}
				std::cout << std::endl;
				};
			lua.set_function("clear", &ng::Editor::ConsoleView::Clear);
			lua.set_function("load", &LuaManager::Load);
			lua.set_function("execute", &LuaManager::Execute);
			// lua.set_function("noclip", &LuaManager::LoadNoclip);

		}

		// Temp method to enable noclip via dev console
		// void LuaManager::LoadNoclip() {
		// 	LuaManager::Load("D:/Projects/CPP/smallengine/res/scripts/noclip.lua");
		// }

		std::vector<LuaScript> LuaManager::s_scripts;

		/// <summary>
		/// Loads a script file and binds it to the active scene.
		/// </summary>
		/// <param name="scene">Scene* reference</param>
		/// <param name="filepath">Path to lua script</param>
		void LuaManager::Load(const char* filepath) {
			
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

			Game* instance = Game::GetInstance();

			script.lua["game"] = instance;
			script.lua["camera"] = instance->GetSceneManager()->GetActiveScene()->GetActiveCamera();

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
		
			RegisterGame(lua);
			RegisterScene(lua);
			RegisterGameObject(lua);
			RegisterTransform(lua);
			RegisterShaderLoader(lua);
			RegisterMeshRenderer(lua);
			RegisterPhysicsBody(lua);
			RegisterSphereCollider(lua);
			RegisterCamera(lua);
			RegisterDeveloperConsole(lua);

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

		/// <summary>
		/// Executes lua code
		/// </summary>
		/// <param name="luaCode">lua code to execute</param>
		void LuaManager::Execute(std::string luaCode)
		{
			LuaScript cmdScript = LuaScript();
			sol::state& lua = cmdScript.lua;

			lua.open_libraries(
				sol::lib::base,
				sol::lib::package,
				sol::lib::math,
				sol::lib::table,
				sol::lib::string
			);

			BindToLua(lua);

			try {
				// Run lua code
				sol::load_result script = lua.load(luaCode);

				if (!script.valid()) {
					sol::error err = script;
					Debug::Log(ERROR, "Compile Error: %s", err.what());
				}
				else {
					sol::protected_function func = script;
					sol::protected_function_result result = func();

					if (!result.valid()) {
						sol::error err = result;
						Debug::Log(WARN, "Runtime Error: %s", err.what());
					}

				}
			}
			catch (const std::exception& e) {
				Debug::Log(ERROR, "Exception: %s", e.what());
			}

		}
	}