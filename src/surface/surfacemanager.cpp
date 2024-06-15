#include "surfacemanager.h"
#include "../log.h"
#include "surface.h" // Include here for complete type information

std::vector<std::shared_ptr<Surface>> SurfaceManager::activeSurfaces;
bool SurfaceManager::isApplicationShuttingDown = false;  // Define and initialize the static bool

void SurfaceManager::registerSurface(const std::shared_ptr<Surface>& surface) {
    activeSurfaces.push_back(surface);
    Log::console("Surface added. Total surfaces: " + std::to_string(activeSurfaces.size()));
}

void SurfaceManager::unregisterSurface(const std::shared_ptr<Surface>& surface) {
    activeSurfaces.erase(std::remove(activeSurfaces.begin(), activeSurfaces.end(), surface), activeSurfaces.end());
    Log::console("Surface removed. Total surfaces: " + std::to_string(activeSurfaces.size()));
}