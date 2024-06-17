#ifndef BODYBOX_H
#define BODYBOX_H

#include <glm/vec3.hpp>

class BodyBox {
public:
    glm::vec3 position;  // Center position of the box
    glm::vec3 size;      // Dimensions of the box (extends in each direction from the center)
    glm::vec3 velocity;  // Velocity vector of the box
    float mass;          // Mass of the box

    BodyBox(const glm::vec3& position, const glm::vec3& size, const glm::vec3& velocity, float mass);
    BodyBox();

    // Check if this box intersects with another box
    bool intersects(const BodyBox& other) const;

    // Resolve collision modifying the velocity of this box based on another box
    void resolveCollision(BodyBox& other);

    // Methods to set position and velocity
    void setPosition(const glm::vec3& newPosition);
    void setVelocity(const glm::vec3& newVelocity);

    glm::vec3 getOverlapCenter(const BodyBox& other) const;
};

#endif // BODYBOX_H
