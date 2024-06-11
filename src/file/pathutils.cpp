#include "pathutils.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

std::filesystem::path getExecutablePath() {
#ifdef _WIN32
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    std::filesystem::path exePath = std::filesystem::path(path).parent_path();
#else
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    std::filesystem::path exePath = std::filesystem::path(result).parent_path();
#endif
    std::cerr << "Executable Path: " << exePath << std::endl; // Print the executable path for debugging
    return exePath;
}

std::filesystem::path getAssetPath(const std::string& relativePath) {
    std::filesystem::path execPath = getExecutablePath();
    std::filesystem::path basePath = execPath.parent_path().parent_path(); // Navigate up two levels to project root
    std::filesystem::path assetPath = basePath / "assets" / relativePath;
    std::cerr << "Asset Path: " << assetPath << std::endl; // Print the asset path for debugging
    return assetPath;
}
