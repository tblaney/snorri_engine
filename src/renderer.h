#pragma once
#include "shader.h"
#include "compute.h"
#include "object/point.h"
#include "object/object.h"

class Renderer : public Component {
private:
    unsigned int VAO, VBO, EBO;
    unsigned int texture;
    Shader shader;
    ComputeShader compute;

public:
    Renderer(Object* parent);

    void loadFromJson(const nlohmann::json& json) override;
    void update() override;

    void createSolidColorTexture(int width, int height, const glm::vec3& color);
    void render(glm::mat4 view, glm::mat4 projection);
};