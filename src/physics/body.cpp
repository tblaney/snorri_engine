#include "body.h"
#include "physicsmanager.h"
#include "../time.h"
#include "../object/point.h"
#include "../surface/surfacemanager.h"
#include "../log.h"
#include <glm/glm.hpp>
#include <iostream>

Body::Body(Object* parent) 
    : Component(parent), shapeType(0), bounciness(0), mass(0.0), desiredVelocity(glm::vec3(0,0,0)),
        currentVelocity(glm::vec3(0,0,0)) {
    Log::console("new body!");
}

Body::~Body() {
    if (!SurfaceManager::isApplicationShuttingDown)
        PhysicsManager::unregisterBody(shared_from_this());
}

void Body::loadFromJson(const nlohmann::json& json) {
    PhysicsManager::registerBody(shared_from_this());
    
    if (json.contains("mass")) {
        mass = json["mass"];
    }
}

void Body::update() {
    //Point& p = getPoint();
    //glm::vec3 pos = p.getPosition();
    //p.setPosition(glm::vec3(pos.x+Time::delta, pos.y, pos.z));

    Point& p = getPoint();
    glm::vec3 pos = p.getPosition();
    std::vector<std::shared_ptr<Body>> bodies = PhysicsManager::getClosest(pos.x, pos.y, pos.z, shared_from_this());
    for (const auto& body : bodies) {
        // Perform cleanup or other operations
        // Example cleanup operation
    }
}

glm::vec4 Body::getVelocity() {
    return glm::vec4(0,0,0,0);
}

BodyData Body::getData() {
    BodyData data;
    const Point& p = getPoint();
    data.position = glm::vec4(p.getPosition(),1);
    glm::vec3 rotationDegrees = p.getRotation();
    data.rotation = glm::vec4(glm::radians(rotationDegrees), 1);
    data.scale = glm::vec4(p.getScale(),1);
    data.velocity = getVelocity();
    data.shapeType = shapeType;
    data.bounciness = bounciness;
    data.mass = mass;
    return data;
}

glm::vec3 Body::getPosition() {
    return getPoint().getPosition();
}

bool body_registered = []() {
    try {
        Log::console("body registering now");
        Object::registerComponent("body", [](Object* parent) {
            return std::make_shared<Body>(parent);
        });
    } catch (const std::exception& e) {
        Log::console(std::string("Failed to register body: ") + e.what());
        return false;
    }
    return true;
}();
