#include "object.h"
#include <nlohmann/json.hpp> // Include the JSON library
#include <iostream>

// Default constructor
Object::Object() : point() {}

// Parameterized constructor
Object::Object(const Point& point) : point(point) {}

// JSON constructor
Object::Object(const JDict& json) {
    loadFromJson(json);
}

// Getter
Point Object::getPoint() const {
    return point;
}

// Setter
void Object::setPoint(const Point& point) {
    this->point = point;
}

// Load from JSON
void Object::loadFromJson(const JDict& json) {
    loadPoint(json.getDict("point"));
    //loadComponents(json.get("components", JDict()));
    //loadChildren(json.get("children", JDict()));
}

// Load Point data from JSON
void Object::loadPoint(const JDict& json) {
    if (json.contains("position") && json.get("position").is_array() && json.get("position").size() == 3) {
        glm::vec3 pos = json.getVec("position");
        point.setPosition(pos);
    }
    if (json.contains("rotation") && json.get("rotation").is_array() && json.get("rotation").size() == 3) {
        glm::vec3 rot = json.getVec("rotation");
        point.setRotation(rot);
    }
    if (json.contains("scale") && json.get("scale").is_array() && json.get("scale").size() == 3) {
        glm::vec3 s = json.getVec("scale");
        point.setScale(s);
    }
}

// Load Components from JSON
void Object::loadComponents(const nlohmann::json& json) {
    for (auto& componentJson : json.items()) {
        std::string type = componentJson.key();
        nlohmann::json componentData = componentJson.value();
        
        auto it = componentFactories.find(type);
        if (it != componentFactories.end()) {
            std::shared_ptr<Component> component = it->second(this);
            component->loadFromJson(componentData);
            components.push_back(component);
        } else {
            std::cerr << "Unknown component type: " << type << std::endl;
        }
    }
}

// Load Children from JSON
void Object::loadChildren(const nlohmann::json& json) {
    for (auto& childJson : json.items()) {
        std::string name = childJson.key();
        nlohmann::json childData = childJson.value();
        
        std::shared_ptr<Object> child = std::make_shared<Object>(childData);
        children[name] = child;
    }
}

// Update (no longer a pure virtual function)
void Object::update() {
    // Default implementation can be empty or contain generic update logic
}

// Initialize static member
std::unordered_map<std::string, Object::ComponentFactory> Object::componentFactories;

// Register component factory
void Object::registerComponent(const std::string& name, ComponentFactory factory) {
    componentFactories[name] = factory;
}
