#pragma once

#include <string>
#include <sstream>
#include <filesystem>

#include <map>
#include <string>
#include <memory>

namespace ng::Assets {


	class Utility {

	public:
		constexpr unsigned int string_hash(const char* s, int off = 0) {
			return !s[off] ? 5381 : (string_hash(s, off + 1) * 33) ^ s[off];
		}

		static std::string GetFullPathFromFilename(const std::string& fileName, std::string filename) {
			std::filesystem::path fullpath = std::filesystem::path(fileName).parent_path() / filename;
			return fullpath.string();
		}
	};
}