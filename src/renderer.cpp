#include "renderer.h"
#include "light.h"
#include "camera/camera.h"
#include "file/pathutils.h"
#include "object/object.h"
#include "log.h"
#include <iostream>
#include <glm/glm.hpp>
#include "surface/surface.h"
#include "surface/surfacemanager.h"
#include "time.h"

// Constructor
Renderer::Renderer(Object* parent) 
    : Component(parent), shader(), compute() {
}

void Renderer::loadFromJson(const nlohmann::json& json) {

    shader.setShaderPaths(getAssetPath(json["vert_shader"]).string(), getAssetPath(json["frag_shader"]).string());
    compute.setShaderPath(getAssetPath(json["compute_shader"]).string());

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

    if (json.contains("width") && json.contains("height")) {
        createSolidColorTexture(json["width"], json["height"], glm::vec3(1.0f, 0.0f, 0.0f));
    } else {
        createSolidColorTexture(640, 360, glm::vec3(1.0f, 0.0f, 0.0f));
    }
}

void Renderer::createSolidColorTexture(int width, int height, const glm::vec3& color) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Prepare color data for the texture
    unsigned char* data = new unsigned char[width * height * 3];
    for (int i = 0; i < width * height; ++i) {
        data[i * 3 + 0] = static_cast<unsigned char>(color.r * 255);
        data[i * 3 + 1] = static_cast<unsigned char>(color.g * 255);
        data[i * 3 + 2] = static_cast<unsigned char>(color.b * 255);
    }

    // Create texture
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] data;
}

void Renderer::setupSurfaceBuffer() {
    std::vector<SurfaceData> surfaceDatas;
    for (auto& surface : SurfaceManager::activeSurfaces) {
        SurfaceData data;
        data.position = surface->getPosition(); // Ensure Surface has a getPosition() method
        surfaceDatas.push_back(data);
    }
    // Log::console("Setting up surface buffer with " + std::to_string(surfaceDatas.size()) + " surfaces.");
    compute.setupSurfaceBuffer(surfaceDatas);
    compute.setInt("numSurfaces", static_cast<int>(surfaceDatas.size()));
}
void Renderer::setupComputeData(Camera* camera, Light* light) {
    compute.setMat4("cameraToWorld", camera->cameraToWorldMatrix);
    compute.setMat4("cameraInverseProjection", camera->cameraInverseProjectionMatrix);
    compute.setVec3("cameraForward", camera->getForward());
    compute.setFloat("time", Time::since);
    compute.setVec3("lightDirection", light->getDirection());
    compute.setVec3("lightPosition", light->getPosition());
}

// Render function
void Renderer::render(Camera* cam, Light* light) {
    glClear(GL_COLOR_BUFFER_BIT);

    compute.use();
    setupComputeData(cam, light);
    setupSurfaceBuffer(); // Binding compute buffer with surface data to image unit 0
    compute.setupResultBuffer(640*360);
    compute.setTexture(texture, 1);  // Binding the texture to image unit 1
    compute.dispatch(640, 360, 1);

    std::vector<ResultData> results = compute.retrieveResults(1024);
    compute.printResults(results);  // Print the results

    shader.use();

    glBindTexture(GL_TEXTURE_2D, texture);

    shader.setInt("texture1", 0); // Set the texture uniform to use texture unit 0

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::update() {
    Camera* cam = Camera::getMainCamera();
    if (cam == nullptr)
        return;
    Light* light = Light::getMainLight();
    if (light == nullptr)
        return;
    render(cam, light);
}

bool renderer_registered = []() {
    try {
        Log::console("renderer registering now");
        Object::registerComponent("renderer", [](Object* parent) {
            return std::make_shared<Renderer>(parent);
        });
    } catch (const std::exception& e) {
        Log::console(std::string("Failed to register renderer: ") + e.what());
        return false;
    }
    return true;
}();
