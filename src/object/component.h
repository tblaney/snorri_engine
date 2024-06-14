#ifndef COMPONENT_H
#define COMPONENT_H

#include "point.h"
#include <nlohmann/json.hpp> // Include the JSON library

class Object; // Forward declaration

class Component {
public:
    Component(Object* parent) : parent(parent) {}
    virtual ~Component() {}

    virtual void loadFromJson(const nlohmann::json& json) = 0;
    virtual void update() {}

protected:

    Point getPoint() const;

    Object* parent;
};

#endif // COMPONENT_H
