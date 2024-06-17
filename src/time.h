#ifndef TIME_H
#define TIME_H

#include <chrono>

class Time {
public:
    static void initialize();  // Initializes the start time
    static bool update();      // Updates the time since startup and returns true if delta is reached
    static bool updatePhysics(); // Updates physics time step

    static float delta;        // Fixed time step for updates in seconds
    static float deltaPhysics; // Fixed physics time step in seconds
    static float since;        // Time since startup in seconds
    static float timeSinceLastPhysicsUpdate; // Time since last physics update

private:
    static std::chrono::high_resolution_clock::time_point startTime;
    static std::chrono::high_resolution_clock::time_point lastUpdate; // Time of the last update
    static std::chrono::high_resolution_clock::time_point lastPhysicsUpdate; // Time of the last physics update
};

#endif // TIME_H
