#include "window.h"
#include "shader.h"
#include "scene.h"

#include "file/pathutils.h"
#include "file/jsonloader.h"
#include "surface/surfacemanager.h"

#include "object/object.h"
#include "object/point.h"

#include "camera/camera.h"

#include "time.h"

#include <iostream> 

int main() {
    
    Window window(640, 360, "snorri_engine_v1");

    Time::initialize();  // Initialize the timer

    Scene scene("data/scene_start.json");

    while (!window.shouldClose()) {
        window.clear();
        
        if (Time::update())  // Update the time each frame
            scene.update();

        window.display();
        window.pollEvents();
    }

    SurfaceManager::isApplicationShuttingDown = true;

    return 0;
}
