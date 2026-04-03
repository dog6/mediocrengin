#include <glad/glad.h>
#include "AVGNG/Core/KeyboardInput.hpp"
#include "AVGNG/Utilities/Debug.hpp"

#include "AVGNG/UI/EditorUI.hpp"
#include "AVGNG/UI/InspectorView.hpp"
#include "AVGNG/UI/HierarchyView.hpp"
#include "AVGNG/UI/ConsoleView.hpp"
#include "AVGNG/UI/MenuStripView.hpp"

using namespace ng::Core;

namespace ng::Editor {

	bool EditorUI::s_isInspectorVisible;
	bool EditorUI::s_isConsoleVisible;
	bool EditorUI::s_isHierarchyVisible;
	bool EditorUI::s_isMenuStripVisible;
	bool EditorUI::s_allVisible;
	// Inspector View
	void EditorUI::ShowInspector()
	{
		Debug::Log(DEBUG, "Showing Inspector View");
		InspectorView::Show();
	}

	void EditorUI::HideInspector()
	{
		Debug::Log(DEBUG, "Hiding Inspector View");
		InspectorView::Hide();
	}


	// Console View
	void EditorUI::ShowConsole()
	{
		Debug::Log(DEBUG, "Showing Developer Console View");
		ConsoleView::Show();
	}

	void EditorUI::HideConsole() {
		Debug::Log(DEBUG, "Hiding Developer Console View");
		ConsoleView::Hide();
	}


	// Hierarchy View
	void EditorUI::HideHierarchy()
	{
		Debug::Log(DEBUG, "Hiding Hierarchy View");
		HierarchyView::Hide();
	}

	void EditorUI::ShowHierarchy()
	{
		Debug::Log(DEBUG, "Showing Hierarchy View");
		HierarchyView::Show();
	}

	// MenuStrip View
	void EditorUI::ShowMenuStrip() {
		Debug::Log(DEBUG, "Showing Menu Strip View");
		MenuStripView::Show();
	}

	void EditorUI::HideMenuStrip() {
		Debug::Log(DEBUG, "Hiding Menu Strip View");
		MenuStripView::Hide();
	}

	// All Views
	void EditorUI::ShowAllElements()
	{
		Debug::Log(DEBUG, "Showing All Editor UI Elements");
		ShowInspector();
		ShowHierarchy();
		ShowConsole();
		ShowMenuStrip();

	}

	void EditorUI::HideAllElements()
	{
		Debug::Log(DEBUG, "Hiding All Editor UI Elements");
		HideInspector();
		HideHierarchy();
		HideConsole();
		HideMenuStrip();
	}


	// Handler Methods
	void EditorUI::Update()
	{

		// Get states
		s_isHierarchyVisible = HierarchyView::IsVisible();
		s_isInspectorVisible = InspectorView::IsVisible();
		s_isMenuStripVisible = MenuStripView::IsVisible();
		s_isConsoleVisible = ConsoleView::IsVisible();
		
		// Update if visible
		if (s_isHierarchyVisible) HierarchyView::Update();
		if (s_isInspectorVisible) InspectorView::Update();
		if (s_isConsoleVisible) ConsoleView::Update();
		if (s_isMenuStripVisible) MenuStripView::Update();

		// ~ to toggle all UI elements
		if (KeyboardInput::IsKeyPressed(Key::KEY_GRAVE_ACCENT)) {

			s_allVisible = !s_allVisible;
			
			if (s_allVisible) {
				ShowAllElements();
			}
			else {
				HideAllElements();
			}

		}

		// Hierarchy
		if (KeyboardInput::IsKeyPressed(Key::KEY_F1)) {
			if (s_isHierarchyVisible) HideHierarchy();
			else ShowHierarchy();
		}

		// Inspector
		if (KeyboardInput::IsKeyPressed(Key::KEY_F2)) {
			if (InspectorView::IsVisible()) HideInspector();
			else ShowInspector();
		}

		// Console
		if (KeyboardInput::IsKeyPressed(Key::KEY_F3)) {
			if (ConsoleView::IsVisible()) HideConsole();
			else ShowConsole();
		}

		// MenuStrip
		if (KeyboardInput::IsKeyPressed(Key::KEY_F4)) {
			if (MenuStripView::IsVisible()) HideMenuStrip();
			else ShowMenuStrip();
		}

	}

}