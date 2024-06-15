#ifndef TIME_H
#define TIME_H

#include <chrono>

class Time {
public:
    static void update();     // Updates the time since startup
    static void initialize(); // Initializes the start time

    static float since;       // Time since startup in seconds

private:
    static std::chrono::high_resolution_clock::time_point startTime; // Start time
};

#endif // TIME_H
