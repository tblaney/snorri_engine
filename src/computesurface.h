#pragma once
#include <string>
#include <iostream>
#include <vector>  // Include for std::vector
#include "compute.h"  // Include for std::vector

class ComputeShaderSurface : public ComputeShader {
public:
    ComputeShaderSurface();

    void setupSurfaceBuffer(const std::vector<SurfaceData>& surfaces);

};
