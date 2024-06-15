#pragma once

#include <vector>
#include <memory>

class Surface; // Forward declaration

class SurfaceManager {
public:
    static std::vector<std::shared_ptr<Surface>> activeSurfaces;

    static void registerSurface(const std::shared_ptr<Surface>& surface);
    static void unregisterSurface(const std::shared_ptr<Surface>& surface);
};