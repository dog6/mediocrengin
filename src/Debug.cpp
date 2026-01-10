#include <AVGNG/Debug.hpp>

namespace ng::Core {


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
			Debug::Log(LOG, "Opened log file '%s'.", filePath.c_str());
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
		va_end(args);


	}


	void Debug::WriteConsole(LogLevel level, const char* msg, va_list args)
	{

#ifndef NG_DEBUG_MODE
		// skip debug-level logs if NG_DEBUG_MODE is not defined
		if (level == DEBUG) return;
#endif

		// Print prefix
		printf("%s", GetLogLevelAsString(level));

		// Print the formatted message
		vprintf(msg, args);

		// Newline at the end
		printf("\n");
	}

	void Debug::WriteFile(LogLevel level, const char* msg, va_list args)
	{

#ifndef NG_DEBUG_MODE
		// skip debug-level logs if NG_DEBUG_MODE is not defined
		if (level == DEBUG) return;
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
