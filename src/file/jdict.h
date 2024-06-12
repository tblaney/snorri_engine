#ifndef JDICT_H
#define JDICT_H

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class JDict {
public:
    JDict();
    JDict(const std::string& jsonString);
    JDict(const nlohmann::json& jsonObj);

    // Set a value in the JSON object
    template<typename T>
    void set(const std::string& key, const T& value);

    // Get a value from the JSON object
    template<typename T>
    T get(const std::string& key, T defaultVal);

    // Get the length of the JSON object
    size_t length() const;

    // Iterate over the JSON object
    std::vector<std::string> keys() const;

    // Convert the JSON object to a string
    std::string toString() const;

private:
    nlohmann::json jsonObj;
};

#endif // JDICT_H
