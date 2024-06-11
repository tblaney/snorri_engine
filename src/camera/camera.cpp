#include "camera.h"

Camera::Camera(const Point& startPoint, glm::vec3 startFront, glm::vec3 startUp, float fov, float aspectRatio, float nearPlane, float farPlane) 
    : Object(startPoint), front(startFront), up(startUp) {
    point = startPoint;  // Assign the startPoint to the point member of Object
    glm::vec3 position = point.getPosition();
    viewMatrix = glm::lookAt(position, position + front, up);
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::update() {
    glm::vec3 position = point.getPosition();
    viewMatrix = glm::lookAt(position, position + front, up);
}
