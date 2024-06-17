#include "window.h"
#include "scene.h"
#include "surface/surfacemanager.h"
#include "time.h"

#include <iostream> 
#include <thread>
#include <atomic>

int main() {
    
    Window window(640, 360, "snorri_engine_v1");

    Time::initialize();

    Scene scene("data/scene_start.json");

    std::atomic<bool> running(true);
    std::thread physicsThread([&]() {
        while (running) {
            if (Time::updatePhysics()) {
                scene.updatePhysics();
            }
        }
    });

    while (!window.shouldClose()) {
        window.clear();

        if (Time::update()) 
            scene.update();

        scene.updateRender();

        window.display();
        window.pollEvents();
    }

    running = false;
    physicsThread.join();  

    SurfaceManager::isApplicationShuttingDown = true;

    return 0;
}

