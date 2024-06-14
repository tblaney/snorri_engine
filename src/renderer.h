#pragma once
#include "shader.h"
#include "object/object.h"
#include <glm/glm.hpp>

class Renderer {
public:
    Renderer(const std::string& vertexPath, const std::string& fragmentPath, const std::string& computePath);

    void loadTexture();
    void loadAndRunComputeShader();
    void render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
    void checkShaderCompilation(GLuint shader);
    void checkProgramLinking(GLuint program);

private:
    unsigned int VAO, VBO, EBO;
    unsigned int texture;
    Shader shader;
    std::string computePath;
};