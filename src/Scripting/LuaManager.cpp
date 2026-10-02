
#include "AVGNG/Scripting/LuaManager.hpp"

using namespace ng::Core;
using namespace ng::Graphics;

namespace ng::Scripting {

		//// Helper Methods
		//// Register Lua Bindings
		
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
			script.lua["camera"] = SceneManager::GetActiveScene()->GetActiveCamera();

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

			Debug::Log(LOG, " -> Finished loading Lua script: '%s'\n\n", filepath);
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
		
			CoreRegistrar::Register(lua);
			GraphicsRegistrar::Register(lua);
			PhysicsRegistrar::Register(lua);
			InputRegistrar::Register(lua);

			RegisterDeveloperConsole(lua);

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