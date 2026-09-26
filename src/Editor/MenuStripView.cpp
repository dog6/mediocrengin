#include "AVGNG/Core/Debug.hpp"
#include "AVGNG/Core/Game.hpp"
#include "AVGNG/Assets/SceneJsonSerializer.hpp"
#include "AVGNG/Editor/MenuStripView.hpp"
#include "AVGNG/Editor/InspectorView.hpp"
// #include <AVGNG/Scene.hpp>

#include <imgui/imgui.h>

using namespace ng::Core;
using namespace ng::Assets;

bool MenuStripView::s_isVisible = false;

void MenuStripView::CreateUI()
{

	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New Scene")) {
				// New Scene
			}
			if (ImGui::MenuItem("Load last save")) {
				// TODO: Create ImGui Open Dialog; for now reload the active scene's file
				// Actually just reloading the current scene is also useful, so we'll add the opening dialog in the future as another option
				Scene* activeScene = Game::GetActiveScene();

				std::string load_path = std::string("D:/Projects/CPP/smallengine/res/scenes/") + activeScene->GetName() + ".json";
				Debug::Log(LOG, "Opening scene file '%s'", load_path.c_str());

				Scene* loadedScene = SceneJsonSerializer::DeserializeSceneFromJson(load_path.c_str(), Game::camera, Game::GetViewportSize());

				if (loadedScene != nullptr) {
					// the old scene's objects are about to be deleted, drop editor references to them
					ng::Editor::InspectorView::Inspect(-1, nullptr);
					SceneManager::SetActiveScene(loadedScene);
				}
			}
			if (ImGui::MenuItem("Reload scripts")) {
				Scene* activeScene = Game::GetActiveScene();
				
				
			}
			if (ImGui::MenuItem("Save Scene")) {
				// Get active scene
				Scene* activeScene = Game::GetActiveScene();

				// Save scene
				Debug::Log(LOG, "Saving active scene '%s'", activeScene->GetName().c_str());

				std::string save_path = std::string("D:/Projects/CPP/smallengine/res/scenes/") + activeScene->GetName();

				SceneJsonSerializer::SerializeSceneToJson(*activeScene, save_path.c_str());

			}
			ImGui::EndMenu();
		}

		// TODO: Implement Edit menu actions
		//if (ImGui::BeginMenu("Edit")) {
		//	if (ImGui::MenuItem("Undo")) {
		//		// Undo action
		//	}
		//	if (ImGui::MenuItem("Redo")) {
		//		// Redo action
		//	}
		//	ImGui::EndMenu();
		//}


		ImGui::EndMainMenuBar();
	}

}
