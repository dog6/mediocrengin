#include <AVGNG/SceneJsonSerializer.hpp>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

using namespace ng::Core;

namespace ng::Assets {

	// TODO: Finish implementing scene saving
	void SceneJsonSerializer::SerializeSceneToJson(Scene& scene, const char* filepath)
	{

		auto sceneGameObjects = scene.GetGameObjects();
		std::string scene_name = scene.GetName();

		json jsonData;

		jsonData["scene"]["sceneName"] = scene_name;

		// Save scene camera data
		ng::Graphics::Camera cam = *scene.GetActiveCamera();
		glm::vec3 camPos = cam.GetPosition();
		glm::vec3 camTarget = cam.GetTarget();

		jsonData["scene"]["sceneCamera"]["position"] = {camPos.x, camPos.y, camPos.z};
		jsonData["scene"]["sceneCamera"]["target"] = {camTarget.x, camTarget.y, camTarget.z};
		
		// Save sceneGameObject data
		int goIndex = 1;
		int compIndex = 1;
		for (auto go : sceneGameObjects) {
			
			// save gameObject info
			jsonData["scene"]["gameObjects"][goIndex] = nlohmann::json::object();
			json& goJSON = jsonData["scene"]["gameObjects"][goIndex];

			goJSON["name"] = go->name;
			goJSON["active"] = go->isActive;

			json& compJSON = goJSON["components"][compIndex];

			auto components = go->GetAttachedComponents();
			for (auto comp : components) {
				// pass jsonData obj to component so it can save
				comp->Save(compJSON, goIndex);
				compIndex++;
			}
			goIndex++;
			compIndex = 1;
		}

		// Write JSON to file
		// TODO: Create ImGui Save Dialog
		// for now we're just saving in the "./res/scenes/" directory.
		std::string scene_fullpath = std::string("./res/scenes/") + scene_name + std::string(".json");
		std::string jsonContent = jsonData.dump(4, ' ', false, json::error_handler_t::ignore);

		FileReader::WriteFile(scene_fullpath, jsonContent);

		Debug::Log(LOG, "Saved scene %s in path '%s'", scene_name.c_str(), scene_fullpath.c_str());

	}

	void SceneJsonSerializer::DeserializeSceneFromJson(const char* filePath)
	{

	}

}