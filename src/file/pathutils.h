#ifndef PATH_UTILS_H
#define PATH_UTILS_H

#include <string>
#include <filesystem>

std::filesystem::path getExecutablePath();
std::filesystem::path getAssetPath(const std::string& relativePath);

#endif // PATH_UTILS_H
