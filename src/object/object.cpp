#include "object.h"

// Default constructor
Object::Object() : point() {}

// Parameterized constructor
Object::Object(const Point& point) : point(point) {}

// Getter
Point Object::getPoint() const {
    return point;
}

// Setter
void Object::setPoint(const Point& point) {
    this->point = point;
}
