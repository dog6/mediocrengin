#pragma once

namespace ng::Editor {

	class EditorUI {

		static bool s_isInspectorVisible;
		static bool s_isConsoleVisible;
		static bool s_isHierarchyVisible;

	public:
		static void ShowInspector();
		static void ShowHierarchy();
		static void ShowConsole();

		static void HideInspector();
		static void HideHierarchy();
		static void HideConsole();

		static void ShowAllElements();
		static void HideAllElements();

		static void Update();

	};

}