#pragma once
#include "AVGNG/Editor/EditorUIElement.hpp"
#include "AVGNG/Core/GameObject.hpp"
#include <vector>

namespace ng::Editor {
	class HierarchyView : public EditorUIElement {

	private:
		static bool s_isVisible;
		static void CreateUI(); // updates UI elements

		static std::vector<ng::Core::GameObject*> gameObjects;
	
	public:

		static void SetGameObjects(std::vector<ng::Core::GameObject*> gameObjects) { HierarchyView::gameObjects = gameObjects; } // sets game objects to be displayed

		static void EditorUIElement::Show()  { s_isVisible = true; }
		static void EditorUIElement::Hide() { s_isVisible = false; }
		static bool EditorUIElement::IsVisible() { return s_isVisible; }
		static void EditorUIElement::Update() { CreateUI(); }

		
	};
}