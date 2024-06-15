#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

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

private:
    std::string readShaderFile(const std::string& filePath);
    void checkCompileErrors(GLuint shader, std::string type);
};
