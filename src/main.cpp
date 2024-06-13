#include "window.h"
#include "shader.h"

#include "file/pathutils.h"
#include "file/jsonloader.h"
#include "file/jdict.h"

#include "object/object.h"
#include "object/point.h"

#include "camera/camera.h"

#include <iostream> 
#include <filesystem>
#include <nlohmann/json.hpp>

int main() {

    Window window(640, 360, "snorri_engine_v1");

    std::filesystem::path jsonPath = getAssetPath("data/scene_start.json");
    JDict dict("scene_start.json");
    dict.log();
    //JDict childDict = dict.get("point");
    //childDict.log();
    Object object(dict);
    
    //while (!window.shouldClose()) {
    //    window.clear();

        //renderer.render();

    //    window.display();
    //    window.pollEvents();
    //}

    return 0;
}
