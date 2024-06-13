#pragma once
#include "../object/point.h"
#include "../object/object.h"
#include <glm/glm.hpp>

class Camera : public Component {
public:
    Camera(Object* parent, glm::vec3 startFront, glm::vec3 startUp, float fov, float aspectRatio, float nearPlane, float farPlane);
    Camera(Object* parent);
    void updateAspectRatio(float aspectRatio);

    void loadFromJson(const nlohmann::json& json) override;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    float aspectRatio;

private:
    glm::vec3 front;
    glm::vec3 up;
};
