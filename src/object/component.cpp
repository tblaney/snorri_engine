#include "component.h"
#include "object.h"
#include "point.h"

// Implementation of the protected getPoint method
Point Component::getPoint() const {
    if (parent) {
        return parent->getPoint();
    }
    return Point(); // Return a default Point if parent is null
}
