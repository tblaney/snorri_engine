#ifndef OBJECT_H
#define OBJECT_H

#include "point.h"
#include "component.h"
#include <nlohmann/json.hpp> // Include the JSON library
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>

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
    void loadFromJson(const nlohmann::json& json);

    // Update (now a normal member function)
    virtual void update();

    // Component management
    template<typename T>
    std::shared_ptr<T> getComponent() const;

    using ComponentFactory = std::function<std::shared_ptr<Component>(Object*)>;
    static std::unordered_map<std::string, ComponentFactory> componentFactories;
    static void registerComponent(const std::string& name, ComponentFactory factory);

protected:
    Point point;
    std::vector<std::shared_ptr<Component>> components;
    std::unordered_map<std::string, std::shared_ptr<Object>> children;

    void loadPoint(const nlohmann::json& json);
    void loadComponents(const nlohmann::json& json);
    void loadChildren(const nlohmann::json& json);
};

template<typename T>
std::shared_ptr<T> Object::getComponent() const {
    for (const auto& component : components) {
        std::shared_ptr<T> casted = std::dynamic_pointer_cast<T>(component);
        if (casted) {
            return casted;
        }
    }
    return nullptr;
}

#endif // OBJECT_H
