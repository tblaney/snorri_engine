#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"
#include "camera/camera.h"
#include <vector>

class Renderer {
private:
    unsigned int VAO, VBO, EBO;
    unsigned int texture;
    Shader shader;
    Camera camera;
    float vertices[20]; // Adjust the size if necessary
    unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 }; // Initialize here since they are constant

public:
    Renderer(const Camera& camera, const std::string& vertexPath, const std::string& fragmentPath);

    void updateVerticesBasedOnCamera();
    void updateVertices();

    void loadTexture(const char* path);
    void render() const;
};

#endif // RENDERER_H
