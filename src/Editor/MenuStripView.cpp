#include "AVGNG/Core/Debug.hpp"
#include "AVGNG/Core/Game.hpp"
#include "AVGNG/Assets/SceneJsonSerializer.hpp"
#include "AVGNG/Editor/MenuStripView.hpp"
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
			if (ImGui::MenuItem("Open Scene")) {
				// Open Scene
			}
			if (ImGui::MenuItem("Save Scene")) {
				// Get active scene
				Scene* activeScene = Game::activeScene;

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
