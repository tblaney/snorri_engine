#include "bodybox.h"
#include <algorithm>
#include <glm/glm.hpp>

BodyBox::BodyBox(const glm::vec3& position, const glm::vec3& size, const glm::vec3& velocity, float mass)
    : position(position), size(size), velocity(velocity), mass(mass) {}


BodyBox::BodyBox() {}

bool BodyBox::intersects(const BodyBox& other) const {
    return (abs(position.x - other.position.x) <= (size.x + other.size.x) * 0.5f) ||
           (abs(position.y - other.position.y) <= (size.y + other.size.y) * 0.5f) ||
           (abs(position.z - other.position.z) <= (size.z + other.size.z) * 0.5f);
}

void BodyBox::resolveCollision(BodyBox& other) {
    glm::vec3 distance = other.position - position;
    glm::vec3 halfSize = size * 0.5f + other.size * 0.5f;
    glm::vec3 overlap = halfSize - glm::abs(distance);

    if (overlap.x < overlap.y && overlap.x < overlap.z) {
        float correction = overlap.x * (distance.x > 0 ? 1 : -1);
        float impulse = correction / (mass + other.mass);
        velocity.x -= impulse * other.mass;
        other.velocity.x += impulse * mass;
    } else if (overlap.y < overlap.z) {
        float correction = overlap.y * (distance.y > 0 ? 1 : -1);
        float impulse = correction / (mass + other.mass);
        velocity.y -= impulse * other.mass;
        other.velocity.y += impulse * mass;
    } else {
        float correction = overlap.z * (distance.z > 0 ? 1 : -1);
        float impulse = correction / (mass + other.mass);
        velocity.z -= impulse * other.mass;
        other.velocity.z += impulse * mass;
    }
}

void BodyBox::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

void BodyBox::setVelocity(const glm::vec3& newVelocity) {
    velocity = newVelocity;
}
