#pragma once
#include "../object/point.h"
#include "../object/object.h"
#include <glm/glm.hpp>

class Camera : public Object {
public:
    Camera(const Point& startPoint, glm::vec3 startFront, glm::vec3 startUp, float fov, float aspectRatio, float nearPlane, float farPlane);

    void update() override;
    void updateAspectRatio(float aspectRatio);

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    float aspectRatio;

private:
    glm::vec3 front;
    glm::vec3 up;
};
