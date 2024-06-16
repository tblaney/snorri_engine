#ifndef SURFACE_H
#define SURFACE_H

#include "../object/object.h"
#include <memory>  // Required for std::enable_shared_from_this
#include <glm/glm.hpp>  // Ensure you have included the GLM library for vector types

struct SurfaceData {
    glm::vec4 position;  // Position of the surface
    glm::vec4 rotation;
    glm::vec4 scale;
    glm::vec4 diffuse;
    int shapeType;
    int blendType;
    float blendStrength;
    float pad;
};

class Surface : public Component, public std::enable_shared_from_this<Surface> {
public:
    Surface(Object* parent);
    ~Surface();

    void loadFromJson(const nlohmann::json& json) override;
    void update() override;

    SurfaceData getData();

    glm::vec3 getPosition();

private:
    glm::vec3 color;
    int shapeType;
    int blendType;
    float blendStrength;
};

#endif // SURFACE_H
