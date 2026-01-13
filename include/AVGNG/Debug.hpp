#pragma once

// DEFINE NG_DEBUG_MODE in preprocessor to enable debug logging
//#define NG_DEBUG_MODE
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <fstream>

namespace ng::Core {

	enum LogLevel {
		LOG,
		DEBUG,
		WARN,
		ERROR,
		FATAL
	};

	class Debug {

		static std::ofstream s_file;

		static void WriteConsole(LogLevel level, const char* msg, va_list args);
		static void WriteFile(LogLevel level, const char* msg, va_list args);
		static const char* GetLogLevelAsString(LogLevel level);
		static bool OpenLogFile(const char* logFilePath);

	public:
		static void Init(const std::string& filePath);     // opens log file
		static void Shutdown();                       // closed log file
		static void Log(LogLevel level, const char* msg, ...);

	};


}