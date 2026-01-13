#include <AVGNG/InspectorView.hpp>

#include <AVGNG/Game.hpp>
#include <vector>
#include <imgui.h>

using namespace ng::Core;

namespace ng::Editor {

	bool InspectorView::s_isVisible = false;

	void InspectorView::CreateUI()
	{

		if (!s_isVisible) return;

		ImGui::Begin("Inspector", &s_isVisible, ImGuiWindowFlags_MenuBar);

		ImGui::TextColored(ImVec4(1, 1, 0, 1), "GameObjects");
		ImGui::BeginChild("Scrolling");

		std::vector<GameObject*> gameObjects = Game::activeScene->GetGameObjects();

		for (int n = 0; n < gameObjects.size(); n++) {
			ImGui::TextColored(ImVec4(0, 1, 1, 1), "GameObject %d: %s", n, gameObjects[n]->name.c_str());
		}

		ImGui::EndChild();

		ImGui::End();

	}

}