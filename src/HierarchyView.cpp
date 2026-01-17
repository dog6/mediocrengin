#include <AVGNG/HierarchyView.hpp>
#include <AVGNG/Game.hpp>

using namespace ng::Core;

namespace ng::Editor {

	std::vector<GameObject*> HierarchyView::gameObjects;

	bool HierarchyView::s_isVisible = false;

	void HierarchyView::CreateUI()
	{

		if (!s_isVisible) return;



		ImGui::Begin("Scene Hierarchy", &s_isVisible, ImGuiWindowFlags_MenuBar);
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Appearing);

		ImGui::TextColored(ImColor(0, 157, 255), "GameObjects");
		ImGui::BeginChild("Scrolling");

		SetGameObjects(Game::activeScene->GetGameObjects());

		std::string currGoLabel = std::string();
		GameObject* currGO;

		for (int n = 0; n < gameObjects.size(); n++) {

			currGO = gameObjects[n]; // get GO name
			currGoLabel.assign("OBJ " + std::to_string(n) + ": " + currGO->name);

			if (ImGui::Button(currGoLabel.c_str(), ImVec2(280, 20))) {

				// pass selected object to inspector
				InspectorView::Inspect(n, gameObjects[n]);

#ifdef NG_DEVELOPER_MODE
				Debug::Log(DEV, "Selected gameObject %s for inspection.", CONSOLE_YELLOW, currGoLabel);
#endif

			}
		}

		ImGui::EndChild();

		ImGui::End();

	}

}