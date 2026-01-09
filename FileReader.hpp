#pragma once


#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <vector>
#include <string>
#include <fstream>

class FileReader {

public:
	static std::vector<std::string> split(std::string_view str, char delim);
	static int safe_stoi(const std::string& s);
	static std::vector<int> split_ints(const std::string& s, const std::string& delimiter);
	static std::ifstream ReadFile(const std::string& filePath);
};



#endif