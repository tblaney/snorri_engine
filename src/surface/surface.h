#ifndef SURFACE_H
#define SURFACE_H

#include "../object/object.h"
#include <memory>  // Required for std::enable_shared_from_this
#include <glm/glm.hpp>  // Ensure you have included the GLM library for vector types

struct SurfaceData {
    glm::vec3 position;  // Position of the surface
};

class Surface : public Component, public std::enable_shared_from_this<Surface> {
public:
    Surface(Object* parent);
    ~Surface();

    void loadFromJson(const nlohmann::json& json) override;
    void update() override;

    glm::vec3 getPosition();

private:
    int type;
};

#endif // SURFACE_H
