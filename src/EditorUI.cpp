#include <AVGNG/EditorUI.hpp>

#include <AVGNG/InspectorView.hpp>
#include <AVGNG/HierarchyView.hpp>

#include <AVGNG/Debug.hpp>

using namespace ng::Core;

namespace ng::Editor {


	bool EditorUI::s_isInspectorVisible;
	bool EditorUI::s_isConsoleVisible;
	bool EditorUI::s_isHierarchyVisible;

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


	void EditorUI::ShowHierarchy()
	{
		Debug::Log(DEBUG, "Showing Hierarchy View");
		HierarchyView::Show();
	}

	void EditorUI::HideHierarchy()
	{
		Debug::Log(DEBUG, "Hiding Hierarchy View");
		HierarchyView::Hide();
	}

	void EditorUI::Update()
	{
		InspectorView::Update();
		HierarchyView::Update();
	}



}