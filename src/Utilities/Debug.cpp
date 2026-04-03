#include "AVGNG/Utilities/Debug.hpp"

#ifdef NG_DEVELOPER_MODE
#include "AVGNG/UI/ConsoleView.hpp"
#endif
#define NG_DEBUG_MODE

namespace ng::Core {


	std::string FormatString(const char* msg, va_list args)
	{
		// Make a copy of args because vsnprintf will consume it
		va_list argsCopy;
		va_copy(argsCopy, args);

		// Compute required size
		int size = std::vsnprintf(nullptr, 0, msg, argsCopy);
		va_end(argsCopy);

		if (size < 0) return ""; // formatting error

		std::vector<char> buffer(size + 1); // +1 for null terminator
		std::vsnprintf(buffer.data(), buffer.size(), msg, args);

		return std::string(buffer.data(), buffer.size() - 1); // remove null terminator
	}

	const char* GetColorCode(LogLevel level)
	{
		switch (level) {
		case LogLevel::LOG:
			return CONSOLE_WHITE;
		case LogLevel::DEBUG:
			return CONSOLE_CYAN;
		case LogLevel::WARN:
			return CONSOLE_YELLOW;
		case LogLevel::ERROR:
			return CONSOLE_RED;
		case LogLevel::FATAL:
			return CONSOLE_MAGENTA;
		case LogLevel::DEV:
			return CONSOLE_GREEN;
		default:
			return CONSOLE_WHITE;
		}
	}

	// Define the static member
	std::ofstream ng::Core::Debug::s_file;

	const char* Debug::GetLogLevelAsString(LogLevel level)
	{

		switch (level) {

		case LogLevel::LOG:
			return "[INFO] ";
		case LogLevel::DEBUG:
			return "[DEBUG] ";
		case LogLevel::WARN:
			return "[WARNING] ";
		case LogLevel::ERROR:
			return "[ERROR] ";
		case LogLevel::FATAL:
			return "[FATAL] ";
		case LogLevel::DEV:
			return "[DEV] ";
		default:
			return "-> ";
		}

	}

	void Debug::Init(const std::string& filePath)
	{
		s_file.open(filePath, std::ios::out | std::ios::trunc);
		bool result = s_file.is_open();
		if (!result) {
			Debug::Log(WARN, "Failed to open log file '%s'. ONLY logs to console will work.", filePath.c_str());
			return;
		}
		else {
			Debug::Log(LOG, "Opened log file '%s'", filePath.c_str());
		}
	}

	void Debug::Shutdown()
	{
		if (s_file.is_open())
			s_file.close();
	}

	// Logs to both file & console
	void Debug::Log(LogLevel level, const char* msg, ...)
	{

		// Format the message once
		char buffer[1024];
		va_list args;
		va_start(args, msg);
		vsnprintf(buffer, sizeof(buffer), msg, args);
		va_end(args);



		const char* cc = GetColorCode(level);
#ifdef NG_DEVELOPER_MODE
		ng::Editor::ConsoleView::Log(buffer);
#endif
		WriteConsole(level, buffer, cc);
		WriteFile(level, buffer);
	}

	void Debug::Log(LogMessage logMessage)
	{
		Log(logMessage.level, CONSOLE_WHITE, logMessage.message.c_str());
	}


	void Debug::WriteConsole(LogLevel level, const char* msg, const char* asciiColorCode)
	{

#ifndef NG_DEBUG_MODE
		// skip debug-level logs if NG_DEBUG_MODE is not defined
		if (level == DEBUG) return;
#endif

#ifndef NG_DEVELOPER_MODE
		// skip dev-level logs if NG_DEVELOPER_MODE is not defined
		if (level == DEV) return;
#endif
		// Print prefix
		printf("%s %s\n", GetLogLevelAsString(level), msg);
	}

	void Debug::WriteFile(LogLevel level, const char* msg)
	{

#ifndef NG_DEBUG_MODE
		// skip debug-level logs if NG_DEBUG_MODE is not defined
		if (level == DEBUG) return;
#endif

#ifndef NG_DEVELOPER_MODE
		if (level == DEV) return;
#endif
		if (!s_file.is_open()) {
			printf("[ERROR] Failed to open log file\n");
			return;
		}

		// Print prefix
		s_file << GetLogLevelAsString(level);

		// Write formatted message to the file
		s_file << msg << std::endl;

	}

}
