#ifndef JDICT_H
#define JDICT_H

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <glm/vec3.hpp>

class JDict {
public:
    JDict();
    JDict(const std::string& jsonFilename);

    // Set a value in the JSON object
    template<typename T>
    void set(const std::string& key, const T& value);

    // Get a value from the JSON object
    template<typename T>
    T get(const std::string& key, T defaultVal);

    bool contains(const std::string& key) const;

    void setDict(const std::string& key, const JDict& value);
    JDict getDict(const std::string& key, const JDict& defaultValue = JDict()) const;

    void setVec(const std::string& key, const glm::vec3& value);
    glm::vec3 getVec(const std::string& key, const glm::vec3& defaultValue) const;

    size_t length() const;
    std::vector<std::string> keys() const;
    std::string toString() const;

    void log() const;

private:
    nlohmann::json jsonObj;
    
};

#endif // JDICT_H
