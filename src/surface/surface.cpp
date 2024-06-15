#include "surface.h"
#include "surfacemanager.h"
#include "../log.h"

Surface::Surface(Object* parent) 
    : Component(parent), type(0) {
    //SurfaceManager::registerSurface(shared_from_this());
    Log::console("new surface!");
}

void Surface::loadFromJson(const nlohmann::json& json) {
    SurfaceManager::registerSurface(shared_from_this());
}

void Surface::update() {

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
