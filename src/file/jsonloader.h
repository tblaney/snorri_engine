#ifndef JSON_LOADER_H
#define JSON_LOADER_H

#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>  // Include the JSON library header

class JsonLoader {
public:
    static nlohmann::json loadJsonFile(const std::filesystem::path& filePath);
};

#endif // JSON_LOADER_H
