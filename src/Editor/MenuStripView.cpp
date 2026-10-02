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


void MenuStripView::FileOptions()
{
	if (ImGui::BeginMenu("File")) {

		
		// New Scene
		if (ImGui::MenuItem("New Scene")) {
		}

		// Reload last saved scene
		if (ImGui::MenuItem("Load last save")) {
			Scene* activeScene = SceneManager::GetActiveScene();

			std::string load_path = std::string("D:/Projects/CPP/smallengine/res/scenes/") + activeScene->GetName() + ".json";
			Debug::Log(LOG, "Opening scene file '%s'", load_path.c_str());

			Scene* loadedScene = SceneJsonSerializer::DeserializeSceneFromJson(load_path.c_str(), Game::camera, Game::GetViewportSize());

			if (loadedScene != nullptr) {
				// the old scene's objects are about to be deleted, drop editor references to them
				ng::Editor::InspectorView::Inspect(-1, nullptr);
				SceneManager::RequestSceneChange(loadedScene);
			}
		}

		// Reloads active scenes assigned lua scripts
		if (ImGui::MenuItem("Reload scripts")) {
			// Scene* activeScene = Game
			// Not implemented yet
			
		}
		
		// TODO: Create ImGui Open and Save Dialog
		if (ImGui::MenuItem("Save Scene")) {
			SaveDialog();
		}
		ImGui::EndMenu();
	}
}

void MenuStripView::SaveDialog()
{
	static char sceneNameBuffer[128] = "";
	static bool showSaveSceneDialog = false;
	if (ImGui::MenuItem("Save Scene")) {
		Scene* activeScene = SceneManager::GetActiveScene();

		// Fill the input box with the current scene name.
		// This lets the user keep the same name, or type a new one.
		strncpy(sceneNameBuffer, activeScene->GetName().c_str(), sizeof(sceneNameBuffer) - 1);
		sceneNameBuffer[sizeof(sceneNameBuffer) - 1] = '\0';

		showSaveSceneDialog = true;
		ImGui::OpenPopup("Save Scene");
	}

	if (ImGui::BeginPopupModal("Save Scene", &showSaveSceneDialog, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Scene Name:");
		ImGui::InputText("##SceneName", sceneNameBuffer, sizeof(sceneNameBuffer));

		ImGui::Separator();

		if (ImGui::Button("Save")) {
			std::string newName = sceneNameBuffer;

			if (!newName.empty()) {
				Scene* activeScene = SceneManager::GetActiveScene();
				activeScene->SetName(newName);

				Debug::Log(LOG, "Saving active scene '%s'", activeScene->GetName().c_str());
				std::string save_path = std::string("D:/Projects/CPP/smallengine/res/scenes/") + activeScene->GetName();
				SceneJsonSerializer::SerializeSceneToJson(*activeScene, save_path.c_str());
			} else {
				Debug::Log(WARN, "Scene name is empty. Save canceled.");
			}

			showSaveSceneDialog = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			showSaveSceneDialog = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void MenuStripView::CreateUI()
{

	if (ImGui::BeginMainMenuBar()) {

		FileOptions();

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
