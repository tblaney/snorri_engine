#ifndef TIME_H
#define TIME_H

#include <chrono>

class Time {
public:
    static bool update();     // Updates the time since startup and returns true if delta is reached
    static void initialize(); // Initializes the start time

    static float delta;        // Fixed time step for updates in seconds
    static float since;        // Time since startup in seconds

private:
    static std::chrono::high_resolution_clock::time_point startTime;
    static std::chrono::high_resolution_clock::time_point lastUpdate; // Time of the last update
};

#endif // TIME_H
