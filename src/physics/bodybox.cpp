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

glm::vec3 BodyBox::getOverlapCenter(const BodyBox& other) const {
    glm::vec3 halfSize1 = size * 0.5f;
    glm::vec3 halfSize2 = other.size * 0.5f;

    glm::vec3 min1 = position - halfSize1;
    glm::vec3 max1 = position + halfSize1;
    glm::vec3 min2 = other.position - halfSize2;
    glm::vec3 max2 = other.position + halfSize2;

    glm::vec3 overlap_start = glm::max(min1, min2);
    glm::vec3 overlap_end = glm::min(max1, max2);
    glm::vec3 overlap_center = (overlap_start + overlap_end) * 0.5f;

    return overlap_center;
}

void BodyBox::resolveCollision(BodyBox& other) {
    if (((position.y - other.position.y) <= (size.y + other.size.y) * 0.5f)) {
        //below
        if (velocity.y < 0.0)
            velocity.y = 0.0;
    } else if ((abs(position.y - other.position.y) <= (size.y + other.size.y) * 0.5f)) {
        // above
        velocity.y = 0.0;
    }
}

void BodyBox::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

void BodyBox::setVelocity(const glm::vec3& newVelocity) {
    velocity = newVelocity;
}
