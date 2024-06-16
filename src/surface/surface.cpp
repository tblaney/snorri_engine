#include "surface.h"
#include "surfacemanager.h"
#include "../log.h"
#include <glm/glm.hpp>
#include <iostream>

Surface::Surface(Object* parent) 
    : Component(parent), shapeType(0), blendType(0), blendStrength(0.0), color(glm::vec3(1,1,0)) {
    Log::console("new surface!");
}

Surface::~Surface() {
    if (!SurfaceManager::isApplicationShuttingDown)
        SurfaceManager::unregisterSurface(shared_from_this());
}

void Surface::loadFromJson(const nlohmann::json& json) {
    SurfaceManager::registerSurface(shared_from_this());
    
    if (json.contains("color")) {
        color = glm::vec3(json["color"][0]/255.0, json["color"][1]/255.0, json["color"][2]/255.0);
    }
    if (json.contains("shape")) {
        shapeType = json["shape"];
    }
    if (json.contains("blend")) {
        blendType = json["blend"];
    }
    if (json.contains("blend_strength")) {
        blendStrength = json["blend_strength"];
    }
}

void Surface::update() {

}

SurfaceData Surface::getData() {
    SurfaceData data;
    const Point& p = getPoint();
    data.position = glm::vec4(p.getPosition(),1);
    glm::vec3 rotationDegrees = p.getRotation();
    data.rotation = glm::vec4(glm::radians(rotationDegrees), 1);
    data.scale = glm::vec4(p.getScale(),1);
    //std::cout << "Surface Scale: " << data.scale.x << ", " << data.scale.y << ", " << data.scale.z << std::endl;
    data.diffuse = glm::vec4(color,1);
    data.blendType = blendType;
    data.shapeType = shapeType;
    data.blendStrength = blendStrength;
    data.outline = 0;
    return data;
}

glm::vec3 Surface::getPosition() {
    return getPoint().getPosition();
}

bool surface_registered = []() {
    try {
        Log::console("surface registering now");
        Object::registerComponent("surface", [](Object* parent) {
            return std::make_shared<Surface>(parent);
        });
    } catch (const std::exception& e) {
        Log::console(std::string("Failed to register renderer: ") + e.what());
        return false;
    }
    return true;
}();
