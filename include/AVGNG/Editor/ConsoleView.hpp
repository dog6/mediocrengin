#pragma once
#include "AVGNG/Editor/EditorUIElement.hpp"
#include "AVGNG/Editor/ConsoleView.hpp"
#include <vector>
#include <string>
#include <imgui/imgui.h>
#include "AVGNG/Scripting/LuaManager.hpp"

namespace ng::Editor 
{

	class ConsoleView : public EditorUIElement {

		private:
			static bool s_isVisible;
			static void CreateUI(); // updates UI elements

			static std::vector<std::string> s_logs;
			static int DeveloperCommandSent(ImGuiInputTextCallbackData* data);

		public:

			static void Log(const char* msg);
			static void Clear();

			static void EditorUIElement::Show() { s_isVisible = true; }
			static void EditorUIElement::Hide() { s_isVisible = false; }
			static bool EditorUIElement::IsVisible() { return s_isVisible; }
			static void EditorUIElement::Update() { CreateUI(); }

	};

}
