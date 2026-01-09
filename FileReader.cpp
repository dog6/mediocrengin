#include "FileReader.hpp"

// Helper methods
std::vector<std::string> FileReader::split(std::string_view str, char delim)
{
    std::vector<std::string> result;
    auto left = str.begin();
    for (auto it = left; it != str.end(); ++it)
    {
        if (*it == delim)
        {
            result.emplace_back(&*left, it - left);
            left = it + 1;
        }
    }
    if (left != str.end())
        result.emplace_back(&*left, str.end() - left);
    return result;
}

int FileReader::safe_stoi(const std::string& s)
{
    try {
        return std::stoi(s);
    }
    catch (const std::exception&) {
        printf("stoi failed on token: '%s'\n", s.c_str());
        return 0;
    }
}

std::vector<int> FileReader::split_ints(const std::string& s, const std::string& delimiter)
{
    std::vector<int> tokens;
    size_t start = 0;
    size_t pos = 0;

    while ((pos = s.find(delimiter, start)) != std::string::npos)
    {
        std::string part = s.substr(start, pos - start);
        tokens.push_back(part.empty() ? 0 : safe_stoi(part));
        start = pos + delimiter.length();
    }

    std::string part = s.substr(start);
    tokens.push_back(part.empty() ? 0 : safe_stoi(part));

    return tokens;
}

std::ifstream FileReader::ReadFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        printf("Failed to open file %s\n", filePath.c_str());
    }
    return file;
}
