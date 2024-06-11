#include "jsonloader.h"
#include <iostream>
#include <fstream>

nlohmann::json JsonLoader::loadJsonFile(const std::filesystem::path& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open JSON file: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }

    nlohmann::json jsonData;
    file >> jsonData;
    return jsonData;
}
