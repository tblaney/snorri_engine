#ifndef POINT_H
#define POINT_H

#include <glm/glm.hpp>

class Point {
public:
    // Constructors
    Point();
    Point(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

    // Getters
    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;
    glm::vec3 getScale() const;

    // Setters
    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);
    void setScale(const glm::vec3& scale);

private:
    glm::vec3 position_;
    glm::vec3 rotation_;
    glm::vec3 scale_;
};

#endif // POINT_H
