#pragma once

#include "shader.h"
#include "computesurface.h"
#include "camera/camera.h"
#include "light.h"
#include "object/point.h"
#include "object/object.h"

class Renderer : public Component {
private:
    unsigned int VAO, VBO, EBO;
    unsigned int texture;
    Shader shader;
    ComputeShaderSurface compute;
    float outlineWidth;

public:
    Renderer(Object* parent);
    ~Renderer() override;

    void loadFromJson(const nlohmann::json& json) override;
    void update() override;

    void createSolidColorTexture(int width, int height, const glm::vec3& color);
    void setupSurfaceBuffer();
    void setupComputeData(Camera* camera, Light* light);
    void render(Camera* camera, Light* light);
};