#include "time.h"

// Initialize static members
std::chrono::high_resolution_clock::time_point Time::startTime;
float Time::since = 0.0f;

void Time::initialize() {
    startTime = std::chrono::high_resolution_clock::now();
}

void Time::update() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
    since = duration.count() / 1000.0f;  // Convert milliseconds to seconds
}
