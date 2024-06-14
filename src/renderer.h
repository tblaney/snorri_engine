#pragma once
#include "shader.h"
#include "compute.h"

class Renderer {
private:
    unsigned int VAO, VBO, EBO;
    unsigned int texture;
    Shader shader;
    ComputeShader compute;

public:
    Renderer(const std::string& vertexPath, const std::string& fragmentPath, const std::string& computePath);

    void createSolidColorTexture(int width, int height, const glm::vec3& color);
    void render(glm::mat4 view, glm::mat4 projection);
};