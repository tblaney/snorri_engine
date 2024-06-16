#ifndef BODY_H
#define BODY_H

#include "../object/object.h"
#include <memory>  // Required for std::enable_shared_from_this
#include <glm/glm.hpp>  // Ensure you have included the GLM library for vector types

struct BodyData {
    glm::vec4 position;  // Position of the surface
    glm::vec4 rotation;
    glm::vec4 scale;
    glm::vec4 velocity;
    int shapeType;
    float bounciness;
    float mass;
    int isFrameChecked;
};

class Body : public Component, public std::enable_shared_from_this<Body> {
public:
    Body(Object* parent);
    ~Body();

    void loadFromJson(const nlohmann::json& json) override;
    void update() override;

    glm::vec4 getVelocity();

    BodyData getData();

    glm::vec3 getPosition();

private:
    int shapeType;
    float bounciness;
    float mass;

    glm::vec3 desiredVelocity;
    glm::vec3 currentVelocity;

    bool isGravity;
};

#endif // BODY_H
