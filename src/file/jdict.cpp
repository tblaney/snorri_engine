#include "jdict.h"
#include "pathutils.h"
#include "jsonloader.h"
#include <iostream>

JDict::JDict() : jsonObj(nlohmann::json::object()) {}

JDict::JDict(const std::string& jsonFilename) {
    std::filesystem::path jsonPath = getAssetPath("data/" + jsonFilename);
    jsonObj = JsonLoader::loadJsonFile(jsonPath);
}

bool JDict::contains(const std::string& key) const {
    return jsonObj.contains(key);
}

void JDict::setDict(const std::string& key, const JDict& value) {
    jsonObj[key] = nlohmann::json::parse(value.toString());
}
void JDict::setVec<glm::vec3>(const std::string& key, const glm::vec3& value) {
    jsonObj[key] = {{"x", value.x}, {"y", value.y}, {"z", value.z}};
}
template<typename T>
void JDict::set(const std::string& key, const T& value) {
    jsonObj[key] = value;
}

JDict JDict::getDict(const std::string& key, const JDict& defaultValue) const {
    if (jsonObj.contains(key)) {
        JDict result;
        result.jsonObj = jsonObj[key];
        return result;
    }
    return defaultValue;
}
glm::vec3 JDict::getVec(const std::string& key, glm::vec3 defaultVal) {
    auto it = jsonObj.find(key);
    if (it != jsonObj.end() && it->is_object()) {
        return glm::vec3(
            it->value("x", 0.0f),
            it->value("y", 0.0f),
            it->value("z", 0.0f)
        );
    }
    return defaultVal;
}
template<typename T>
T JDict::get(const std::string& key, T defaultVal) {
    return jsonObj.value(key, defaultVal);
}


size_t JDict::length() const {
    return jsonObj.size();
}

// Log the JSON object to console
void JDict::log() const {
    std::cout << jsonObj.dump(4) << std::endl; // Pretty print with 4 spaces indentation
}

std::vector<std::string> JDict::keys() const {
    std::vector<std::string> keys;
    for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it) {
        keys.push_back(it.key());
    }
    return keys;
}

std::string JDict::toString() const {
    return jsonObj.dump();
}

// Explicitly instantiate the template methods for the types you intend to use
template void JDict::set<int>(const std::string& key, const int& value);
template void JDict::set<float>(const std::string& key, const float& value);
template void JDict::set<double>(const std::string& key, const double& value);
template void JDict::set<std::string>(const std::string& key, const std::string& value);
template void JDict::set<bool>(const std::string& key, const bool& value);

template int JDict::get<int>(const std::string& key, int defaultVal);
template float JDict::get<float>(const std::string& key, float defaultVal);
template double JDict::get<double>(const std::string& key, double defaultVal);
template std::string JDict::get<std::string>(const std::string& key, std::string defaultVal);
template bool JDict::get<bool>(const std::string& key, bool defaultVal);

template void JDict::set<glm::vec3>(const std::string& key, const glm::vec3& value);
