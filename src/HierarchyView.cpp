#include <AVGNG/HierarchyView.hpp>

#include <AVGNG/Game.hpp>

using namespace ng::Core;

bool ng::Editor::HierarchyView::s_isVisible = false;

void ng::Editor::HierarchyView::CreateUI()
{

	if (!s_isVisible) return;

	ImGui::Begin("Scene Hierarchy", &s_isVisible, ImGuiWindowFlags_MenuBar);

	ImGui::TextColored(ImColor(0, 157, 255), "GameObjects");
	ImGui::BeginChild("Scrolling");

	std::vector<GameObject*> gameObjects = Game::activeScene->GetGameObjects();

	for (int n = 0; n < gameObjects.size(); n++) {
		
		if (ImGui::Button(("OBJ #" + std::to_string(n) + ": " + gameObjects[n]->name).c_str(), ImVec2(240, 20))) {
		
			// pass selected object to inspector
			InspectorView::Inspect(gameObjects[n]);

		}
	}

	ImGui::EndChild();

	ImGui::End();

}
