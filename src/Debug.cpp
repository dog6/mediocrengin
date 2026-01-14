#include <AVGNG/Debug.hpp>

#ifdef NG_DEVELOPER_MODE
#include <AVGNG/ConsoleView.hpp>
#endif

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
		if (!OpenLogFile(filePath.c_str())) {
			Debug::Log(WARN, "%sFailed to open log file '%s'. ONLY logs to console will work.", filePath.c_str());
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

	bool Debug::OpenLogFile(const char* logFilePath)
	{
		s_file.open(logFilePath, std::ios::out | std::ios::trunc);
		return s_file.is_open();
	}


	// Logs to both file & console
	void Debug::Log(LogLevel level, const char* msg, ...)
	{

		va_list args;
		va_start(args, msg);
		WriteConsole(level, msg, args);
		WriteFile(level, msg, args);

#ifdef NG_DEVELOPER_MODE
		ng::Editor::ConsoleView::Log(FormatString(msg, args).c_str());
#endif

		va_end(args);

	}

	void Debug::Log(LogLevel level, const char* asciiColorCode, const char* msg, ...) {


		va_list args;
		va_start(args, msg);
		WriteConsole(level, msg, args, asciiColorCode);
		WriteFile(level, msg, args);

#ifdef NG_DEVELOPER_MODE
		ng::Editor::ConsoleView::Log(FormatString(msg, args).c_str());
#endif

		va_end(args);

	}

	void Debug::Log(LogMessage logMessage)
	{
		Log(logMessage.level, "%s", logMessage.message.c_str());
	}


	void Debug::WriteConsole(LogLevel level, const char* msg, va_list args, const char* asciiColorCode)
	{

#ifndef NG_DEBUG_MODE
		// skip debug-level logs if NG_DEBUG_MODE is not defined
		if (level == DEBUG) return;
#endif

#ifndef NG_DEVELOPER_MODE
		if (level == DEV) return;
#endif

		// Print prefix
		printf("%s", GetLogLevelAsString(level));

		// Print the formatted message
		vprintf(msg, args);
		printf("%s", CONSOLE_WHITE);

		// Newline at the end
		printf("\n");

	

	}

	void Debug::WriteFile(LogLevel level, const char* msg, va_list args)
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

		// Format the variadic arguments
		char buff[2048]; // max message size
		vsnprintf(buff, sizeof(buff), msg, args);

		// Write formatted message to the file
		s_file << buff << std::endl;

	}

}
