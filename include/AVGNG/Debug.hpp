#pragma once

#include <string>
#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <fstream>

#include <AVGNG/ConsoleColors.hpp>
#include <AVGNG/ConsoleView.hpp>


namespace ng::Core {

	enum LogLevel {
		LOG,
		DEV,
		DEBUG,
		WARN,
		ERROR,
		FATAL
	};

	class Debug {

		static std::ofstream s_file;

		static void WriteConsole(LogLevel level, const char* msg, va_list args, const char* asciiColorCode = CONSOLE_WHITE);
		static void WriteFile(LogLevel level, const char* msg, va_list args);
		static const char* GetLogLevelAsString(LogLevel level);
		static bool OpenLogFile(const char* logFilePath);

	public:
		static void Init(const std::string& filePath);     // opens log file
		static void Shutdown();                       // closed log file
		static void Log(LogLevel level, const char* msg, ...);
		static void Log(LogLevel level, const char* asciiColorCode, const char* msg, ...);

	};


}