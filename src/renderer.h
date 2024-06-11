#pragma once
#include "shader.h"
#include "camera/camera.h"

class Renderer {
private:
    unsigned int VAO, VBO, EBO;
    unsigned int texture;
    Shader shader;
    Camera camera;

public:
    Renderer(const Camera& camera, const std::string& vertexPath, const std::string& fragmentPath);

    void loadTexture(const char* path);
    void render() const;
};
