#pragma once
#include "../object/object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera : public Object {
public:
    glm::vec3 front;
    glm::vec3 up;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    Camera(const Point& startPoint, glm::vec3 startFront, glm::vec3 startUp, float fov, float aspectRatio, float nearPlane, float farPlane);

    void update() override;
};
