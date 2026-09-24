#include "AVGNG/Assets/SceneJsonSerializer.hpp"
#include "AVGNG/Assets/JsonUtils.hpp"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>

using json = nlohmann::json;

using namespace ng::Core;

namespace ng::Assets {

	// Maps a saved component key to the component instance that should load it.
	// New component types need an entry here (and in GameObject::AddComponentByName).
	static IComponent* GetOrAddComponentForKey(GameObject* go, const std::string& key)
	{
		if (key == "transform") return go->GetComponent<Transform>(); // CreateGameObject already attached one
		if (key == "meshRenderer") return go->AddComponent<ng::Graphics::MeshRenderer>();
		return nullptr;
	}

	void SceneJsonSerializer::SerializeSceneToJson(Scene& scene, const char* filepath)
	{

		std::string scene_name = scene.GetName();

		json jsonData;
		json& sceneJSON = jsonData["scene"];

		sceneJSON["sceneName"] = scene_name;

		// Save scene camera data
		ng::Graphics::Camera* cam = scene.GetActiveCamera();
		if (cam != nullptr) {
			sceneJSON["sceneCamera"]["position"] = Vec3ToJson(cam->GetPosition());
			sceneJSON["sceneCamera"]["target"] = Vec3ToJson(cam->GetTarget());
		}
		else {
			Debug::Log(WARN, "Scene '%s' has no active camera, skipping camera save", scene_name.c_str());
		}

		// Save sceneGameObject data
		json& gameObjectsJSON = sceneJSON["gameObjects"];
		gameObjectsJSON = json::array();

		for (auto* go : scene.GetGameObjects()) {

			json goJSON = json::object();
			goJSON["name"] = go->name;
			goJSON["active"] = go->isActive;

			// each component gets its own entry in the components array
			json& componentsJSON = goJSON["components"];
			componentsJSON = json::array();

			for (auto* comp : go->GetAttachedComponents()) {
				json compJSON = json::object();
				comp->Save(compJSON);
				componentsJSON.push_back(std::move(compJSON));
			}

			gameObjectsJSON.push_back(std::move(goJSON));
		}

		// Write JSON to file
		// TODO: Create ImGui Save Dialog
		std::filesystem::path scene_fullpath(filepath);
		if (scene_fullpath.extension() != ".json") scene_fullpath += ".json";

		std::string jsonContent = jsonData.dump(4, ' ', false, json::error_handler_t::ignore);

		if (!FileReader::WriteFile(scene_fullpath.string(), jsonContent)) {
			Debug::Log(ERROR, "Failed to write scene file '%s'", scene_fullpath.string().c_str());
			return;
		}

		Debug::Log(LOG, "Saved scene %s in path '%s'", scene_name.c_str(), scene_fullpath.string().c_str());

	}

	ng::Core::Scene* SceneJsonSerializer::DeserializeSceneFromJson(const char* filePath, ng::Graphics::Camera* camera, glm::uvec2& viewportSize)
	{

		std::ifstream ifs(filePath);
		if (!ifs.is_open()) {
			Debug::Log(ERROR, "Failed to open scene file '%s'", filePath);
			return nullptr;
		}

		json jf = json::parse(ifs, nullptr, false);
		if (jf.is_discarded() || !jf.contains("scene")) {
			Debug::Log(ERROR, "Failed to parse scene file '%s'", filePath);
			return nullptr;
		}

		const json& sceneJSON = jf.at("scene");
		std::string scene_name = sceneJSON.value("sceneName", "New Scene");

		// Restore scene camera data
		if (camera != nullptr && sceneJSON.contains("sceneCamera")) {
			const json& camJSON = sceneJSON.at("sceneCamera");
			camera->SetPosition(ReadVec3(camJSON, "position", camera->GetPosition()));
			camera->SetTarget(ReadVec3(camJSON, "target", camera->GetTarget()));
		}

		Scene* scene = new Scene(camera, viewportSize, scene_name.c_str());

		if (!sceneJSON.contains("gameObjects")) return scene;

		for (const auto& goJSON : sceneJSON.at("gameObjects")) {

			// older scene files padded the arrays with null entries
			if (!goJSON.is_object()) continue;

			GameObject* go = scene->CreateGameObject(goJSON.value("name", "GameObject"));
			go->isActive = goJSON.value("active", true);

			if (!goJSON.contains("components")) continue;

			for (const auto& compJSON : goJSON.at("components")) {

				if (!compJSON.is_object()) continue;

				// each entry looks like { "<componentKey>": { ...component data... } }
				for (auto it = compJSON.begin(); it != compJSON.end(); ++it) {

					IComponent* comp = GetOrAddComponentForKey(go, it.key());
					if (comp == nullptr) {
						Debug::Log(WARN, "Unknown component '%s' on GameObject '%s', skipping", it.key().c_str(), go->name.c_str());
						continue;
					}

					comp->Load(compJSON);
				}
			}
		}

		Debug::Log(LOG, "Loaded scene '%s' from path '%s'", scene_name.c_str(), filePath);

		return scene;

	}

}
