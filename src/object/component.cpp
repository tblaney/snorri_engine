#include "component.h"
#include "object.h"
#include "point.h"

// Correctly scoped implementation of the getPoint method for the Component class
Point& Component::getPoint() {
    if (parent) {
        return parent->getPoint();
    }
    static Point defaultPoint; // Fixed instance to return by reference
    return defaultPoint; // Return a default Point if parent is null
}

const Point& Component::getPoint() const {
    if (parent) {
        return parent->getPoint();
    }
    static const Point defaultConstPoint; // Fixed const instance to return by reference
    return defaultConstPoint; // Return a default const Point if parent is null
}