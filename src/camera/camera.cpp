#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // Include this header for glm::lookAt and glm::perspective

Camera::Camera(const Point& startPoint, glm::vec3 startFront, glm::vec3 startUp, float fov, float aspectRatio, float nearPlane, float farPlane) 
    : Object(startPoint), front(startFront), up(startUp), aspectRatio(aspectRatio) {
    glm::vec3 position = point.getPosition();
    viewMatrix = glm::lookAt(position, position + front, up);
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::update() {
    glm::vec3 position = point.getPosition();
    viewMatrix = glm::lookAt(position, position + front, up);
}

void Camera::updateAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f); // Example FOV, near, and far planes
}
