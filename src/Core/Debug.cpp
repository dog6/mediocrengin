#include "AVGNG/Core/Debug.hpp"

#include <cstdarg>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>

#ifdef NG_DEVELOPER_MODE
#include "AVGNG/Editor/ConsoleView.hpp"
#endif

// This define affects only this file.
// To enable debug logs in all files, put it in the project settings.
#define NG_DEBUG_MODE

namespace ng::Core {

	// Define the static member
	std::ofstream ng::Core::Debug::s_file;

	std::string FormatString(const char* msg, va_list args)
	{
		// Make a copy of args because vsnprintf consumes it
		va_list argsCopy;
		va_copy(argsCopy, args);

		// Compute the required size
		int size = std::vsnprintf(nullptr, 0, msg, argsCopy);
		va_end(argsCopy);

		if (size < 0) return ""; // formatting error

		std::vector<char> buffer(size + 1); // +1 for the null terminator
		std::vsnprintf(buffer.data(), buffer.size(), msg, args);

		return std::string(buffer.data(), size);
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
		case LogLevel::VERBOSE:
			return CONSOLE_YELLOW;
		default:
			return CONSOLE_WHITE;
		}
	}

	// The only place that decides if a log level is active.
	static bool ShouldLog(LogLevel level)
	{
#ifndef NG_VERBOSE_MODE
		if (level == LogLevel::VERBOSE) return false;
#endif
#ifndef NG_DEBUG_MODE
		if (level == LogLevel::DEBUG) return false;
#endif
#ifndef NG_DEVELOPER_MODE
		if (level == LogLevel::DEV) return false;
#endif
		return true;
	}

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
		case LogLevel::VERBOSE:
			return "[VERBOSE] ";
		default:
			return "-> ";
		}
	}

	void Debug::Init(const std::string& filePath)
	{
		s_file.open(filePath, std::ios::out | std::ios::trunc);

		if (!s_file.is_open()) {
			Debug::Log(WARN, "Failed to open log file '%s'. ONLY logs to console will work.", filePath.c_str());
			return;
		}

		Debug::Log(LOG, "Opened log file '%s'", filePath.c_str());
	}

	void Debug::Shutdown()
	{
		if (s_file.is_open())
			s_file.close();
	}

	// Logs to the console, the file, and the editor console.
	void Debug::Log(LogLevel level, const char* msg, ...)
	{
		if (!ShouldLog(level)) return;

		va_list args;
		va_start(args, msg);
		const std::string text = FormatString(msg, args);
		va_end(args);

#ifdef NG_DEVELOPER_MODE
		ng::Editor::ConsoleView::Log(text.c_str());
#endif
		WriteConsole(level, text.c_str(), GetColorCode(level));
		WriteFile(level, text.c_str());
	}

	void Debug::Log(LogMessage logMessage)
	{
		// The variadic Log() does the filter check.
		// Use "%s" so that the message is never read as a format string.
		Log(logMessage.level, "%s", logMessage.message.c_str());
	}

	void Debug::WriteConsole(LogLevel level, const char* msg, const char* asciiColorCode)
	{
		if (!ShouldLog(level)) return;

		printf("%s %s\n", GetLogLevelAsString(level), msg);
	}

	void Debug::WriteFile(LogLevel level, const char* msg)
	{
		if (!ShouldLog(level)) return;

		// If the file is not open, do nothing.
		// Init() already tells the user about this problem.
		if (!s_file.is_open()) return;

		s_file << GetLogLevelAsString(level) << msg << std::endl;
	}

}