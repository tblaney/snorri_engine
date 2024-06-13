#include "window.h"
#include "shader.h"

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

    std::filesystem::path jsonPath = getAssetPath("data/scene_start.json");
    nlohmann::json config = JsonLoader::loadJsonFile(jsonPath.string());
    Object object(config);
    
    //while (!window.shouldClose()) {
    //    window.clear();

        //renderer.render();

    //    window.display();
    //    window.pollEvents();
    //}

    return 0;
}
