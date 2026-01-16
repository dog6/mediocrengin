#pragma once

#include <vector>
#include <string>
#include <fstream>

#include <AVGNG/Debug.hpp>

namespace ng::Assets {

		class FileReader {

		public:
			static std::vector<std::string> split(std::string_view str, char delim);
			static int safe_stoi(const std::string& s);
			static std::vector<int> split_ints(const std::string& s, const std::string& delimiter);
			static std::string ReadFile(const std::string& filePath);
		};

}

