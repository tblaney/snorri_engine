#pragma once
#include "shader.h"

class Renderer {
private:
    unsigned int VAO, VBO, EBO;
    unsigned int texture;
    Shader shader;

public:
    Renderer(const std::string& vertexPath, const std::string& fragmentPath);

    void loadTexture(const char* path);
    void render(glm::mat4 view, glm::mat4 projection);
};