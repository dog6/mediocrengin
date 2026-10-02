#pragma once
#include <string>

namespace ng::Editor {

class FileDialog {
public:
    FileDialog() = delete;

    // Returns true and sets outPath if the user selects a file.
    static bool OpenImage(std::string& outPath);
};

}