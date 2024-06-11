#include "point.h"

// Default constructor
Point::Point() : position_(0.0f), rotation_(0.0f), scale_(1.0f) {}

// Parameterized constructor
Point::Point(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
    : position_(position), rotation_(rotation), scale_(scale) {}

// Getters
glm::vec3 Point::getPosition() const {
    return position_;
}

glm::vec3 Point::getRotation() const {
    return rotation_;
}

glm::vec3 Point::getScale() const {
    return scale_;
}

// Setters
void Point::setPosition(const glm::vec3& position) {
    position_ = position;
}

void Point::setRotation(const glm::vec3& rotation) {
    rotation_ = rotation;
}

void Point::setScale(const glm::vec3& scale) {
    scale_ = scale;
}
