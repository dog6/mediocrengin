#pragma once

#include "AVGNG/UI/EditorUIElement.hpp"

class MenuStripView : public EditorUIElement {
	static bool s_isVisible;
	static void CreateUI();
public:
	static void EditorUIElement::Show() { s_isVisible = true; }
	static void EditorUIElement::Hide() { s_isVisible = false; }
	static bool EditorUIElement::IsVisible() { return s_isVisible; }
	static void EditorUIElement::Update() { CreateUI(); }

};