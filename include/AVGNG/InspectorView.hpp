#pragma once

#include <AVGNG/EditorUIElement.hpp>
#include <AVGNG/GameObject.hpp>

namespace ng::Editor {

	class InspectorView : public EditorUIElement {

	private:
		static ng::Core::GameObject* s_inspectedObject;
		static bool s_isVisible;
		static void CreateUI(); // updates UI elements

	public:

		static void EditorUIElement::Show() { s_isVisible = true; } // shows UI
		static void EditorUIElement::Hide() { s_isVisible = false; } // hides UI
		static bool EditorUIElement::IsVisible() { return s_isVisible; }
		
		static void Inspect(int objIndex, ng::Core::GameObject* object); // sets object to be inspected

		static void Update() { CreateUI(); }


	};

}