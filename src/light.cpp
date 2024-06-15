#include "light.h"
#include "log.h"
#include "object/object.h"
#include <glm/glm.hpp>
#include <iostream>

Light* Light::mainLight = nullptr;

Light::Light(Object* parent) 
    : Component(parent) {

}

void Light::loadFromJson(const nlohmann::json& json) {
    if (json.contains("is_main") && json["is_main"] == true)
        setMainLight(this);
}

void Light::update() {
}

Light* Light::getMainLight() {
    return mainLight;
}
void Light::setMainLight(Light* light) {
    mainLight = light;
}

glm::vec3 Light::getPosition() {
    return getPoint().getPosition();
}
glm::vec3 Light::getDirection() {
    return getPoint().getFrontDirection();
}



bool light_registered = []() {
    Log::console("light registering now");
    Object::registerComponent("light", [](Object* parent) {
        return std::make_shared<Light>(parent);
    });
    return true;
}();
