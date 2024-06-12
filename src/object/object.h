#ifndef OBJECT_H
#define OBJECT_H

#include "point.h"
#include <nlohmann/json.hpp> // Include the JSON library

class Object {
public:
    // Constructors
    Object();
    Object(const Point& point);
    Object(const nlohmann::json& json); // New constructor

    // Getters
    Point getPoint() const;

    // Setters
    void setPoint(const Point& point);

    // Load from JSON
    virtual void loadFromJson(const nlohmann::json& json);

    // Update
    virtual void update() = 0;

protected:
    Point point;
};

#endif // OBJECT_H
