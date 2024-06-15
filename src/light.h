#pragma once

#include "object/point.h"
#include "object/object.h"
#include <glm/glm.hpp>

class Light : public Component {
public:
    static Light* getMainLight();
    static void setMainLight(Light* light);  // Static method to set the main camera

    Light(Object* parent);

    void loadFromJson(const nlohmann::json& json) override;
    void update() override;

    glm::vec3 getPosition();
    glm::vec3 getDirection();

private:
    static Light* mainLight;  // Static pointer to the main camera
};
