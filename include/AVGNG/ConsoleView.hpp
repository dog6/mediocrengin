#pragma once
#include <AVGNG/EditorUIElement.hpp>
#include <AVGNG/ConsoleView.hpp>
#include <vector>
#include <string>
#include <imgui.h>

namespace ng::Editor 
{

	class ConsoleView : public EditorUIElement {

		private:
			static std::vector<std::string> s_logs;
			static bool s_isVisible;
			static void CreateUI(); // updates UI elements
			static int DeveloperCommandSent(ImGuiInputTextCallbackData* data);

		public:

			static void EditorUIElement::Show() { s_isVisible = true; }
			static void EditorUIElement::Hide() { s_isVisible = false; }
			static bool EditorUIElement::IsVisible() { return s_isVisible; }

			static void Update() { CreateUI(); }
			static void Log(const char* msg);
			static void Clear();

	};

}
