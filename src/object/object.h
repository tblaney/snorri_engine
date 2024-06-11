#ifndef OBJECT_H
#define OBJECT_H

#include "point.h"

class Object {
public:
    // Constructors
    Object();
    Object(const Point& point);

    // Getters
    Point getPoint() const;

    // Setters
    void setPoint(const Point& point);

private:
    Point point;
};

#endif // OBJECT_H
