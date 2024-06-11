#include "window.h"
#include "shader.h"
#include "renderer.h"
#include "file/pathutils.h"
#include "file/jsonloader.h"

#include <iostream> 
#include <filesystem>
#include <nlohmann/json.hpp>

int main() {
    Window window(640, 480, "OpenGL Window");

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
