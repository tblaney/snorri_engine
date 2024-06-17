#pragma once

#include "shader.h"
#include "object/point.h"
#include "object/object.h"
#include <nlohmann/json.hpp>  // Ensure json is properly included if it's used directly

class RendererUi : public Component {
private:
    unsigned int VAO, VBO, EBO;
    unsigned int texture;  // Texture to which we render
    unsigned int fbo;      // Framebuffer object
    Shader shader;
    int width, height;

    void initRenderTexture();  // Initialize rendering texture and framebuffer

public:
    RendererUi(Object* parent);
    ~RendererUi() override;

    void loadFromJson(const nlohmann::json& json) override;
    void updateRender() override;
    void render();

    GLuint getTexture();  // Get the texture for external use
};
