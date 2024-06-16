#include "computesurface.h"
#include "log.h"
#include "surface/surface.h"
#include <glm/gtc/type_ptr.hpp>

ComputeShaderSurface::ComputeShaderSurface() : ComputeShader() {
        
}
void ComputeShaderSurface::setupSurfaceBuffer(const std::vector<SurfaceData>& surfaces) {
    createBuffer(&ssbo, surfaces.size() * sizeof(SurfaceData), surfaces.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo); // Bind to binding point 0
}