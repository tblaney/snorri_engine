#include "window.h"
#include "shader.h"
#include "renderer.h"

#include "file/pathutils.h"
#include "file/jsonloader.h"

#include "object/object.h"
#include "object/point.h"

#include "camera/camera.h"

#include <iostream> 
#include <filesystem>
#include <nlohmann/json.hpp>

int main() {

    // Create a Point instance
    glm::vec3 position(1.0f, 2.0f, 3.0f);
    glm::vec3 rotation(0.0f, 0.0f, 0.0f);
    glm::vec3 scale(1.0f, 1.0f, 1.0f);
    Point cameraPosition(position, rotation, scale);

    // Create a Camera instance
    glm::vec3 startFront(0.0f, 0.0f, -1.0f);
    glm::vec3 startUp(0.0f, 1.0f, 0.0f);
    float fov = 45.0f;
    float aspectRatio = 800.0f / 600.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    Camera camera(cameraPosition, startFront, startUp, fov, aspectRatio, nearPlane, farPlane);

    // Display the Point's attributes
    std::cout << "Initial Point attributes:" << std::endl;
    std::cout << "Position: (" 
              << camera.getPoint().getPosition().x << ", "
              << camera.getPoint().getPosition().y << ", "
              << camera.getPoint().getPosition().z << ")" << std::endl;
    std::cout << "Rotation: (" 
              << camera.getPoint().getRotation().x << ", "
              << camera.getPoint().getRotation().y << ", "
              << camera.getPoint().getRotation().z << ")" << std::endl;
    std::cout << "Scale: (" 
              << camera.getPoint().getScale().x << ", "
              << camera.getPoint().getScale().y << ", "
              << camera.getPoint().getScale().z << ")" << std::endl;


    Window window(640, 360, "snorri_engine_v1");

    // Get the path to the executable's directory
    std::filesystem::path execPath = getExecutablePath();
    std::cerr << "Executable Path: " << execPath << std::endl;

    // Construct the paths to the shader files
    std::filesystem::path vertexPath = getAssetPath("shaders/vertex_shader.glsl");
    std::filesystem::path fragmentPath = getAssetPath("shaders/fragment_shader.glsl");

    std::filesystem::path jsonPath = getAssetPath("data/config.json");
    nlohmann::json config = JsonLoader::loadJsonFile(jsonPath);
    std::cerr << "Loaded JSON: " << config.dump(4) << std::endl;

    std::cerr << "Vertex Shader Path: " << vertexPath << std::endl;
    std::cerr << "Fragment Shader Path: " << fragmentPath << std::endl;

    Shader shader(vertexPath.string(), fragmentPath.string());
    Renderer renderer;

    while (!window.shouldClose()) {
        window.clear();

        shader.use();
        renderer.render();

        window.display();
        window.pollEvents();
    }

    return 0;
}
