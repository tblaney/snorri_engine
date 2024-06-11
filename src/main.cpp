#include "window.h"
#include "shader.h"
#include "renderer.h"

#include "file/pathutils.h"
#include "file/jsonloader.h"

#include "object/object.h"
#include "object/point.h"

#include <iostream> 
#include <filesystem>
#include <nlohmann/json.hpp>

int main() {

    // Create a Point instance
    glm::vec3 position(1.0f, 2.0f, 3.0f);
    glm::vec3 rotation(0.0f, 0.0f, 0.0f);
    glm::vec3 scale(1.0f, 1.0f, 1.0f);
    Point point(position, rotation, scale);

    // Create an Object instance using the Point
    Object obj(point);

    // Display the Point's attributes
    std::cout << "Initial Point attributes:" << std::endl;
    std::cout << "Position: (" 
              << obj.getPoint().getPosition().x << ", "
              << obj.getPoint().getPosition().y << ", "
              << obj.getPoint().getPosition().z << ")" << std::endl;
    std::cout << "Rotation: (" 
              << obj.getPoint().getRotation().x << ", "
              << obj.getPoint().getRotation().y << ", "
              << obj.getPoint().getRotation().z << ")" << std::endl;
    std::cout << "Scale: (" 
              << obj.getPoint().getScale().x << ", "
              << obj.getPoint().getScale().y << ", "
              << obj.getPoint().getScale().z << ")" << std::endl;


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
