#include "AVGNG/Editor/FileDialog.hpp"
#include "AVGNG/Core/Debug.hpp"

// May want to look into NFD library for multi-OS support in the future

#ifdef _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #include <commdlg.h>
#endif

namespace ng::Editor {

bool FileDialog::OpenImage(std::string& outPath)
{
#ifdef _WIN32
    char fileName[MAX_PATH] = {};

    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter =
        "Image files\0*.png;*.jpg;*.jpeg;*.bmp;*.tga\0"
        "All files\0*.*\0";
    ofn.lpstrFile  = fileName;
    ofn.nMaxFile   = MAX_PATH;
    ofn.lpstrTitle = "Select texture";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

    if (!GetOpenFileNameA(&ofn)) return false; // User canceled

    outPath = fileName;
    return true;
#else
    ng::Core::Debug::Log(ng::Core::LogLevel::WARN, "FileDialog is not available on this platform.");
    return false;
#endif
}

}