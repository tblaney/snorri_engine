#include "time.h"

std::chrono::high_resolution_clock::time_point Time::startTime;
std::chrono::high_resolution_clock::time_point Time::lastUpdate;
float Time::delta = 0.005f;  // 30 updates per second
float Time::since = 0.0f;

void Time::initialize() {
    startTime = std::chrono::high_resolution_clock::now();
    lastUpdate = std::chrono::high_resolution_clock::now();
}

bool Time::update() {
    auto now = std::chrono::high_resolution_clock::now();
    auto timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastUpdate).count();
    if (timeSinceLastUpdate >= delta) {
        lastUpdate = now;  // Update the time of the last update
        since = std::chrono::duration_cast<std::chrono::duration<float>>(now - startTime).count();
        return true; // Enough time has passed for an update
    }
    return false; // Not enough time has passed
}
