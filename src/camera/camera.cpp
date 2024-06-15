#define GLM_ENABLE_EXPERIMENTAL

#include "camera.h"
#include "../log.h"
#include "../file/pathutils.h"
#include "../file/jsonloader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // Include this header for glm::lookAt and glm::perspective
#include <glm/ext.hpp>
#include <iostream>

Camera* Camera::mainCamera = nullptr;

Camera::Camera(Object* parent, glm::vec3 startFront, glm::vec3 startUp, float fov, float aspectRatio, float nearPlane, float farPlane) 
    : Component(parent), front(startFront), up(startUp), aspectRatio(aspectRatio), fov(fov), nearPlane(nearPlane), farPlane(farPlane) {
    refreshMatrices();
}
Camera::Camera(Object* parent) 
    : Component(parent) {
    glm::vec3 front(0.0f, 0.0f, -1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    fov = 45.0f;
    aspectRatio = 640.0f / 360.0f;
    nearPlane = 0.1f;
    farPlane = 100.0f;

    refreshMatrices();
}

void Camera::updateAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f); // Example FOV, near, and far planes
}

void Camera::loadFromJson(const nlohmann::json& json) {
    if (json.contains("is_main") && json["is_main"] == true)
        setMainCamera(this);

    // Implement loading logic, for example:
    if (json.contains("front") && json["front"].is_array() && json["front"].size() == 3) {
        front = glm::vec3(json["front"][0], json["front"][1], json["front"][2]);
    }
    if (json.contains("up") && json["up"].is_array() && json["up"].size() == 3) {
        up = glm::vec3(json["up"][0], json["up"][1], json["up"][2]);
    }
    if (json.contains("fov")) {
        float fov = json["fov"];
    }
    if (json.contains("aspectRatio")) {
        aspectRatio = json["aspectRatio"];
    }

    Log::console("camera has loaded from json");
    JsonLoader::printJson(json);
}

Camera* Camera::getMainCamera() {
    return mainCamera;
}
void Camera::setMainCamera(Camera* camera) {
    mainCamera = camera;
}

glm::vec3 Camera::getForward() {
    return getPoint().getFrontDirection();
}

void Camera::update() {
    refreshMatrices();
}

void Camera::refreshMatrices() {
    glm::vec3 position = getPoint().getPosition();
    //std::cout << "camera position: (" 
    //          << position.x << ", " 
    //          << position.y << ", " 
    //          << position.z << ")" << std::endl;
    //glm::vec3 front = getPoint().getFrontDirection();
    //glm::vec3 up = getPoint().getUpDirection();
    glm::vec3 front(0.0f, 0.0f, -1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    viewMatrix = glm::lookAt(position, position + front, up);
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    cameraToWorldMatrix = glm::inverse(viewMatrix);
    cameraInverseProjectionMatrix = glm::inverse(projectionMatrix);
    
    //std::cout << "Camera To World Matrix:" << std::endl;
    //for (int i = 0; i < 4; ++i) {
    //    for (int j = 0; j < 4; ++j) {
    //        std::cout << cameraToWorldMatrix[i][j] << " ";
    //    }
    //    std::cout << std::endl;
    //}
    
}

bool camera_registered = []() {
    Log::console("camera registering now");
    Object::registerComponent("camera", [](Object* parent) {
        return std::make_shared<Camera>(parent);
    });
    return true;
}();
