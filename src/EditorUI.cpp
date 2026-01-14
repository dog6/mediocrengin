#include <AVGNG/EditorUI.hpp>

#include <AVGNG/InspectorView.hpp>
#include <AVGNG/HierarchyView.hpp>
#include <AVGNG/ConsoleView.hpp>

#include <AVGNG/Debug.hpp>

using namespace ng::Core;

namespace ng::Editor {

	bool EditorUI::s_isInspectorVisible;
	bool EditorUI::s_isConsoleVisible;
	bool EditorUI::s_isHierarchyVisible;

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


	// All Views
	void EditorUI::ShowAllElements()
	{
		Debug::Log(DEBUG, "Showing All Editor UI Elements");
		InspectorView::Show();
		HierarchyView::Show();
		ConsoleView::Show();
	}

	void EditorUI::HideAllElements()
	{
		Debug::Log(DEBUG, "Hiding All Editor UI Elements");
		InspectorView::Hide();
		HierarchyView::Hide();
		ConsoleView::Hide();
	}


	// Handler Methods
	void EditorUI::Update()
	{
		InspectorView::Update();
		HierarchyView::Update();
		ConsoleView::Update();
	}

}