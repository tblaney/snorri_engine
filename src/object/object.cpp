#include "object.h"
#include <nlohmann/json.hpp> // Include the JSON library

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
    point.setPosition(glm::vec3(json["position"][0], json["position"][1], json["position"][2]));
    point.setRotation(glm::vec3(json["rotation"][0], json["rotation"][1], json["rotation"][2]));
    point.setScale(glm::vec3(json["scale"][0], json["scale"][1], json["scale"][2]));
}
