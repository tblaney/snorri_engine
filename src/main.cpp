#include "window.h"
#include "shader.h"
#include "scene.h"

#include "file/pathutils.h"
#include "file/jsonloader.h"

#include "object/object.h"
#include "object/point.h"

#include "camera/camera.h"

#include <iostream> 
#include <filesystem>
#include <nlohmann/json.hpp>

int main() {

    Window window(640, 360, "snorri_engine_v1");

    Scene scene("data/scene_start.json");

    while (!window.shouldClose()) {
        window.clear();
        scene.update();
        window.display();
        window.pollEvents();
    }

    return 0;
}
