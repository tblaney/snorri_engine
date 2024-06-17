#ifndef BODY_H
#define BODY_H

#include "../object/object.h"
#include "bodybox.h"
#include <memory>  // Required for std::enable_shared_from_this
#include <glm/glm.hpp>  // Ensure you have included the GLM library for vector types

class Body : public Component, public std::enable_shared_from_this<Body> {
public:
    Body(Object* parent);
    ~Body();

    void loadFromJson(const nlohmann::json& json) override;
    void updatePhysics() override;

    glm::vec3 getVelocity();
    glm::vec3 getPosition();
    BodyBox& getBodyBox();

    bool isKinematic;

private:
    float mass;
    glm::vec3 size;

    BodyBox bodyBox;

    glm::vec3 desiredVelocity;
    glm::vec3 currentVelocity;

    bool isGravity;
};

#endif // BODY_H
