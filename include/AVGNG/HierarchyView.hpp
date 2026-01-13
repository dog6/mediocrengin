#pragma once

#include <AVGNG/EditorUIElement.hpp>

namespace ng::Editor {
	class HierarchyView : EditorUIElement {

	private:
		static bool s_isVisible;
		static void CreateUI(); // updates UI elements

	public:

		static void EditorUIElement::Show() { s_isVisible = true; }
		static void EditorUIElement::Hide() { s_isVisible = false; }
		static bool EditorUIElement::IsVisible() { return s_isVisible; }

		static void Update() { CreateUI(); }

	};
}