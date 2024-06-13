#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // Include this header for glm::lookAt and glm::perspective

Camera::Camera(Object* parent, glm::vec3 startFront, glm::vec3 startUp, float fov, float aspectRatio, float nearPlane, float farPlane) 
    : Component(parent), front(startFront), up(startUp), aspectRatio(aspectRatio) {
    glm::vec3 position = getPoint().getPosition();
    viewMatrix = glm::lookAt(position, position + front, up);
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}
Camera::Camera(Object* parent) 
    : Component(parent) {
    glm::vec3 front(0.0f, 0.0f, -1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    float fov = 45.0f;
    aspectRatio = 640.0f / 360.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    glm::vec3 position = getPoint().getPosition();
    viewMatrix = glm::lookAt(position, position + front, up);
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::updateAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f); // Example FOV, near, and far planes
}

void Camera::loadFromJson(const nlohmann::json& json) {
    // Implement loading logic, for example:
    if (json.contains("front")) {
        front = glm::vec3(json["front"][0], json["front"][1], json["front"][2]);
    }
    if (json.contains("up")) {
        up = glm::vec3(json["up"][0], json["up"][1], json["up"][2]);
    }
    if (json.contains("fov")) {
        float fov = json["fov"];
        projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
    }
    if (json.contains("aspectRatio")) {
        aspectRatio = json["aspectRatio"];
        projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    }
}

bool camera_registered = []() {
    Object::registerComponent("camera", [](Object* parent) {
        return std::make_shared<Camera>(parent);
    });
    return true;
}();
