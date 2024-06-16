#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>  // Include for std::vector
#include "surface/surface.h"  // Include for std::vector
#include <glm/glm.hpp>  // Ensure you have included the GLM library for vector types

struct ResultData {
    glm::vec3 worldPosition;  // Assuming this is the structure used in the SSBO
    float pad;
};

class ComputeShader {
private:
    GLuint programID;

public:
    ComputeShader();
    ~ComputeShader();

    void setShaderPath(const std::string& computePath);

    void use();
    void dispatch(int width, int height, int depth);
    void setTexture(GLuint textureID, GLuint unit);

    void setupSurfaceBuffer(const std::vector<SurfaceData>& surfaces);
    void setupResultBuffer(int numElements);
    std::vector<ResultData> retrieveResults(int numElements);
    void printResults(std::vector<ResultData>& results);

    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setBool(const std::string& name, bool value);
    void setMat4(const std::string& name, const glm::mat4 &mat);
    void setVec3(const std::string& name, const glm::vec3& value);

private:
    std::string readShaderFile(const std::string& filePath);
    void checkCompileErrors(GLuint shader, std::string type);

    void createBuffer(GLuint* buffer, GLsizeiptr size, const void* data, GLenum usage);
    GLuint ssbo;  // Declare the Shader Storage Buffer Object identifier
    GLuint ssboResult;  // Declare the Shader Storage Buffer Object identifier
};
