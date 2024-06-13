#include "object.h"
#include <nlohmann/json.hpp> // Include the JSON library
#include <iostream>

// Default constructor
Object::Object() : point() {}

// Parameterized constructor
Object::Object(const Point& point) : point(point) {}

// JSON constructor
Object::Object(const nlohmann::json& json) {
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
void Object::loadFromJson(const nlohmann::json& json) {
    loadPoint(json["point"]);
    loadComponents(json["components"]);
    loadChildren(json["children"]);
}

// Load Point data from JSON
void Object::loadPoint(const nlohmann::json& json) {
    if (json.contains("position")  && json["rotation"].is_array() && json["rotation"].size() == 3) {
        point.setPosition(glm::vec3(json["position"][0], json["position"][1], json["position"][2]));
    }
    if (json.contains("rotation") && json["rotation"].is_array() && json["rotation"].size() == 3) {
        point.setRotation(glm::vec3(json["rotation"][0], json["rotation"][1], json["rotation"][2]));
    }
    if (json.contains("scale")  && json["rotation"].is_array() && json["rotation"].size() == 3) {
        point.setScale(glm::vec3(json["scale"][0], json["scale"][1], json["scale"][2]));
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
