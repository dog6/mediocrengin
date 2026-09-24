#include "AVGNG/Core/Game.hpp"
#include "AVGNG/Editor/InspectorView.hpp"


using namespace ng::Core;

namespace ng::Editor {

	bool InspectorView::s_isVisible = false;
	GameObject* InspectorView::s_inspectedObject = nullptr;

	std::vector<IComponent*> InspectorView::s_inspectedComponents;

	void InspectorView::CreateUI()
	{
		// TODO:
		// Bug where s_inspectedObject disappears after being selected
		// Looks like we're accidentally taking a ptr to an object somewhere

		if (!s_isVisible) return;
		ImGui::Begin("Inspector", &s_isVisible, ImGuiWindowFlags_MenuBar);
		ImGui::SetWindowPos(ImVec2(100, 0), ImGuiCond_Appearing);

		if (s_inspectedObject != nullptr) {
			ImGui::TextColored(ImColor(0, 157, 255), "Inspecting: %s", s_inspectedObject->name.c_str());

			// Display more details about the inspected object here
			// For example, list its components, properties, etc.
			int n = 0;
			for (auto& comp : s_inspectedComponents) {
				ImGui::PushID(n);
				ImGui::Separator();
				comp->OnInspectorGUI();
				n++;
				ImGui::PopID();
			}

		}
		else {
			ImGui::TextColored(ImColor(255, 0, 0), "No GameObject selected for inspection.");
		}
		ImGui::End();

	}

	void InspectorView::Inspect(int objIndex, Core::GameObject* object)
	{

		s_inspectedComponents.clear();
		s_inspectedObject = object;

		if (s_inspectedObject != nullptr) {
			s_inspectedComponents = object->GetAttachedComponents();
			Debug::Log(DEBUG, "Inspector now looking at %s (%d)", object->name.c_str(), objIndex);
		}
		else {
			Debug::Log(DEBUG, "Inspector no longer inspecting any object.");
		}

	}

}