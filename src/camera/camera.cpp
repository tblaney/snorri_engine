#define GLM_ENABLE_EXPERIMENTAL

#include "camera.h"
#include "../log.h"
#include "../file/pathutils.h"
#include "../file/jsonloader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // Include this header for glm::lookAt and glm::perspective
#include <glm/ext.hpp>
#include <iostream>

Camera::Camera(Object* parent, glm::vec3 startFront, glm::vec3 startUp, float fov, float aspectRatio, float nearPlane, float farPlane) 
    : Component(parent), front(startFront), up(startUp), aspectRatio(aspectRatio), fov(fov), nearPlane(nearPlane), farPlane(farPlane) {
    glm::vec3 position = getPoint().getPosition();
    viewMatrix = glm::lookAt(position, position + front, up);
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}
Camera::Camera(Object* parent) 
    : Component(parent) {
    glm::vec3 front(0.0f, 0.0f, -1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    fov = 45.0f;
    aspectRatio = 640.0f / 360.0f;
    nearPlane = 0.1f;
    farPlane = 100.0f;

    glm::vec3 position = getPoint().getPosition();

    Log::console("new camera at: " + glm::to_string(position));
    
    viewMatrix = glm::lookAt(position, position + front, up);
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::updateAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f); // Example FOV, near, and far planes
}

void Camera::loadFromJson(const nlohmann::json& json) {
    // Implement loading logic, for example:
    if (json.contains("front") && json["front"].is_array() && json["front"].size() == 3) {
        front = glm::vec3(json["front"][0], json["front"][1], json["front"][2]);
    }
    if (json.contains("up") && json["up"].is_array() && json["up"].size() == 3) {
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

    Log::console("camera has loaded from json");
    JsonLoader::printJson(json);

    setupRenderer(json);
}

void Camera::setupRenderer(const nlohmann::json& json) {
    // Implement loading logic, for example:
    Log::console("camera setup renderer");
    Log::console(json["vert_shader"]);

    if (json.contains("vert_shader") && json.contains("frag_shader") && json.contains("compute_shader")) {
        renderer = std::make_unique<Renderer>(
            getAssetPath("shaders/vertex_shader.glsl").string(), 
            getAssetPath("shaders/fragment_shader.glsl").string());
    } else
    {
        Log::console("camera json has no data");
    }
}

void Camera::update() {
    //Log::console(
    //    "camera update!"
    //);
    if (renderer) {
        renderer->render(viewMatrix, projectionMatrix); // Pass this Camera instance to the renderer
    }
}

bool camera_registered = []() {
    Object::registerComponent("camera", [](Object* parent) {
        return std::make_shared<Camera>(parent);
    });
    return true;
}();
