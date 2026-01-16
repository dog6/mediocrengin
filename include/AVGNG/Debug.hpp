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

	struct LogMessage {
		LogLevel level;
		std::string message;
	};

	class Debug {

		static std::ofstream s_file;

		static void WriteConsole(LogLevel level, const char* msg, const char* asciiColorCode = CONSOLE_WHITE);
		static void WriteFile(LogLevel level, const char* msg);
		static const char* GetLogLevelAsString(LogLevel level);
	public:
		static void Init(const std::string& filePath);     // opens log file
		static void Shutdown();                       // closed log file
		static void Log(LogLevel level, const char* msg, ...);
		//static void Log(LogLevel level, const char* asciiColorCode, const char* msg, ...);
		static void Log(LogMessage logMessage);

	};


}