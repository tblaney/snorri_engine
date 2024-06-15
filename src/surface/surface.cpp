#include "surface.h"
#include "surfacemanager.h"
#include "../log.h"
#include <glm/glm.hpp>

Surface::Surface(Object* parent) 
    : Component(parent), type(0) {
    Log::console("new surface!");
}

Surface::~Surface() {
    if (!SurfaceManager::isApplicationShuttingDown)
        SurfaceManager::unregisterSurface(shared_from_this());
}

void Surface::loadFromJson(const nlohmann::json& json) {
    SurfaceManager::registerSurface(shared_from_this());
}

void Surface::update() {

}

glm::vec3 Surface::getPosition() {
    return getPoint().getPosition();
}

bool surface_registered = []() {
    try {
        Log::console("surface registering now");
        Object::registerComponent("surface", [](Object* parent) {
            return std::make_shared<Surface>(parent);
        });
    } catch (const std::exception& e) {
        Log::console(std::string("Failed to register renderer: ") + e.what());
        return false;
    }
    return true;
}();
