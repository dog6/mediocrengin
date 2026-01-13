#include <AVGNG/InspectorView.hpp>

#include <AVGNG/Game.hpp>
#include <vector>
#include <imgui.h>


using namespace ng::Core;

namespace ng::Editor {

	bool InspectorView::s_isVisible = false;
	GameObject* InspectorView::s_inspectedObject = nullptr;

	void InspectorView::CreateUI()
	{

		if (!s_isVisible) return;
		ImGui::Begin("Inspector", &s_isVisible, ImGuiWindowFlags_MenuBar);
		if (s_inspectedObject != nullptr) {
			ImGui::TextColored(ImColor(0, 157, 255), "Inspecting: %s", s_inspectedObject->name.c_str());
			// Display more details about the inspected object here
			// For example, list its components, properties, etc.
		}
		else {
			ImGui::TextColored(ImColor(255, 0, 0), "No GameObject selected for inspection.");
		}
		ImGui::End();

	}

	void InspectorView::Inspect(Core::GameObject* object)
	{
		s_inspectedObject = object;

		if (s_inspectedObject == nullptr) {
			Debug::Log(DEBUG, "Inspector now looking at %s", object->name.c_str());
		}
	}

}