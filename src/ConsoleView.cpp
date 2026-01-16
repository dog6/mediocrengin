#include <AVGNG/ConsoleView.hpp>
#include <AVGNG/Game.hpp>


using namespace ng::Core;

namespace ng::Editor {


	bool ConsoleView::s_isVisible = false;
	std::vector<std::string> ConsoleView::s_logs;

	char cmdBuff[256] = "";

	int ConsoleView::DeveloperCommandSent(ImGuiInputTextCallbackData* data) {

		const char* cmd = data->Buf;

		Debug::Log(DEV, "> %s", cmd);

		// TODO:
		ng::Scripting::LuaManager::Execute(cmd);



		return 0;

	}

	void ConsoleView::CreateUI()
	{

		if (!s_isVisible) return;

		ImGui::Begin("Developer Console", &s_isVisible, ImGuiWindowFlags_MenuBar);
		ImGui::SetWindowPos(ImVec2(0, 100), ImGuiCond_Appearing);

		ImGui::BeginChild("Scrolling", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);

			for (const std::string& log : s_logs) 
				ImGui::TextColored(ImColor(255, 255, 255), "> %s", log.c_str());

			// If already at bottom, keep following new content
			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();

		ImGui::InputText("Command Input", cmdBuff, sizeof(cmdBuff), ImGuiInputTextFlags_CallbackCompletion, DeveloperCommandSent);

		ImGui::End();

		}

	void ConsoleView::Log(const char* msg) {
		s_logs.push_back(std::string(msg));
	}

	void ConsoleView::Clear() {
		s_logs.clear();
	}

}