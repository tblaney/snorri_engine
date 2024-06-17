#include "body.h"
#include "bodybox.h"
#include "physicsmanager.h"
#include "../time.h"
#include "../surface/surfacemanager.h"
#include "../log.h"
#include <glm/glm.hpp>
#include <iostream>

Body::Body(Object* parent) 
    : Component(parent), mass(0.0), size(glm::vec3(1,1,1)), desiredVelocity(glm::vec3(0,0,0)),
        currentVelocity(glm::vec3(0,0,0)), isGravity(false), isKinematic(false), bodyBox() {
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
    } else {
        mass = 1.0;
    }
    if (json.contains("size")) {
        size = glm::vec3(json["size"][0],json["size"][1],json["size"][2]);
    } else {
        size = glm::vec3(1,1,1);
    }
    if (json.contains("is_gravity")) {
        isGravity = json["is_gravity"];
    }
    if (json.contains("is_kinematic")) {
        isKinematic = json["is_kinematic"];
    }

    bodyBox.mass = mass;
    bodyBox.size = size;
    Point& p = getPoint();
    glm::vec3 pos = p.getPosition();
    bodyBox.setPosition(pos);
}

BodyBox& Body::getBodyBox() {
    return bodyBox;
}

void Body::updatePhysics() {
    if (isKinematic)
        return;

    Point& p = getPoint();
    glm::vec3 pos = p.getPosition();
    glm::vec3 vel = getVelocity();

    bodyBox.setPosition(pos);
    bodyBox.setVelocity(vel);

    //std::cout << "Body size is: " << bodyBox.size.x << ", " <<
     //   bodyBox.size.y << ", " << bodyBox.size.z
     //   << std::endl;

    std::vector<std::shared_ptr<Body>> bodies = PhysicsManager::getClosest(pos.x, pos.y, pos.z);
    //Log::console("Number of bodies retrieved for collision checks: " + std::to_string(bodies.size()));
    for (const auto& body : bodies) {
        if (body == shared_from_this())
            continue;

        BodyBox& otherBox = body->getBodyBox();

        if (!body->isKinematic)
            continue;
        //std::cout << "Other body position is: " << otherBox.position.x << ", " <<
        //    otherBox.position.y << ", " << otherBox.position.z
        //    << std::endl;
        if (bodyBox.intersects(otherBox)) {
            //Log::console("body collision registered!");
            bodyBox.resolveCollision(otherBox);
        }
    }
    currentVelocity = bodyBox.velocity;
    pos += bodyBox.velocity * Time::delta; // Ensure deltaTime is defined or accessible
    p.setPosition(pos); // Make sure setPosition actually modifies the Body's stored position, or that it's unnecessary based on your architecture
}

glm::vec3 Body::getVelocity() {
    glm::vec3 vel = currentVelocity;
    vel = glm::mix(vel, desiredVelocity + glm::vec3(0,-9.8,0), Time::delta);
    return vel;
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
