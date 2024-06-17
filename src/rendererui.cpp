#include "rendererui.h"
#include "object/object.h"
#include "log.h"
#include <iostream>
#include <glm/glm.hpp>
#include "time.h"
#include "window.h"
#include "file/pathutils.h"

// Constructor
RendererUi::RendererUi(Object* parent) 
    : Component(parent), shader() {
        initRenderTexture();
}

RendererUi::~RendererUi() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture);
    glDeleteFramebuffers(1, &fbo);
}

void RendererUi::initRenderTexture() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log::console("Framebuffer is not complete");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RendererUi::loadFromJson(const nlohmann::json& json) {
    width = 640;
    height = 360;
    if (json.contains("width"))
        width = json["width"];
    if (json.contains("height"))
        height = json["height"];

    shader.setShaderPaths(getAssetPath(json["vert_shader"]).string(), 
        getAssetPath(json["frag_shader"]).string());

    float vertices[] = {
        // positions       // texture coords
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RendererUi::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);  // Bind the framebuffer to render to the texture
    glViewport(0, 0, width, height);        // Ensure the viewport matches the texture dimensions
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void RendererUi::updateRender() {
    render();
}

GLuint RendererUi::getTexture() {
    return texture;
}


bool rendererui_registered = []() {
    try {
        Log::console("RendererUi registering now");
        Object::registerComponent("renderer_ui", [](Object* parent) {
            return std::make_shared<RendererUi>(parent);
        });
    } catch (const std::exception& e) {
        Log::console(std::string("Failed to register RendererUi: ") + e.what());
        return false;
    }
    return true;
}();
