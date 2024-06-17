#include "time.h"

std::chrono::high_resolution_clock::time_point Time::startTime;
std::chrono::high_resolution_clock::time_point Time::lastUpdate;
std::chrono::high_resolution_clock::time_point Time::lastPhysicsUpdate;
float Time::delta = 0.005f;  // 30 updates per second
float Time::deltaPhysics = 0.016f;  // Physics update rate, approx. 60 updates per second
float Time::since = 0.0f;
float Time::timeSinceLastPhysicsUpdate = 0.0f;

void Time::initialize() {
    startTime = std::chrono::high_resolution_clock::now();
    lastUpdate = std::chrono::high_resolution_clock::now();
    lastPhysicsUpdate = std::chrono::high_resolution_clock::now();
}

bool Time::update() {
    auto now = std::chrono::high_resolution_clock::now();
    auto timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastUpdate).count();
    if (timeSinceLastUpdate >= delta) {
        lastUpdate = now;
        since = std::chrono::duration_cast<std::chrono::duration<float>>(now - startTime).count();
        return true;
    }
    return false;
}

bool Time::updatePhysics() {
    auto now = std::chrono::high_resolution_clock::now();
    timeSinceLastPhysicsUpdate = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastPhysicsUpdate).count();
    if (timeSinceLastPhysicsUpdate >= deltaPhysics) {
        lastPhysicsUpdate = now;
        return true;
    }
    return false;
}
