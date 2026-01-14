#include <AVGNG/ConsoleView.hpp>
#include <AVGNG/Game.hpp>


using namespace ng::Core;

namespace ng::Editor {


	bool ConsoleView::s_isVisible = false;
	std::vector<std::string> ConsoleView::s_logs;


	void ConsoleView::DeveloperCommandSent(const char* msg) {
		Debug::Log(LOG, msg);
	}

	void ConsoleView::CreateUI()
	{

		if (!s_isVisible) return;

		ImGui::Begin("Developer Console", &s_isVisible, ImGuiWindowFlags_MenuBar);

		ImGui::TextColored(ImColor(0, 157, 255), "Output:");


		ImGui::BeginChild("Scrolling", ImVec2(0,0), true);
			for (const std::string& log : s_logs) {
				ImGui::TextColored(ImColor(255, 255, 255), "> %s", log.c_str());
			}

		ImGui::EndChild();


		ImGui::End();

		}

	void ConsoleView::Log(const char* msg) {
		s_logs.push_back(std::string(msg));
	}

	void ConsoleView::Clear() {
		s_logs.clear();
	}

}