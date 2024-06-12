#include "jdict.h"
#include <iostream>

JDict::JDict() : jsonObj(nlohmann::json::object()) {}

JDict::JDict(const std::string& jsonString) {
    jsonObj = nlohmann::json::parse(jsonString);
}

JDict::JDict(const nlohmann::json& jsonObj) : jsonObj(jsonObj) {}

// Set a value in the JSON object
template<typename T>
void JDict::set(const std::string& key, const T& value) {
    jsonObj[key] = value;
}

// Get a value from the JSON object with a default value
template<typename T>
T JDict::get(const std::string& key, T defaultVal) {
    if (jsonObj.contains(key)) {
        return jsonObj[key].get<T>();
    } else {
        return defaultVal;
    }
}

// Get the length of the JSON object
size_t JDict::length() const {
    return jsonObj.size();
}

// Iterate over the JSON object and return keys
std::vector<std::string> JDict::keys() const {
    std::vector<std::string> keys;
    for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it) {
        keys.push_back(it.key());
    }
    return keys;
}

// Convert the JSON object to a string
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
