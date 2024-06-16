#pragma once

#include <vector>
#include <memory>

class Body;

class PhysicsManager {
public:
    static std::vector<std::shared_ptr<Body>> activeBodies;

    static void registerBody(const std::shared_ptr<Body>& body);
    static void unregisterBody(const std::shared_ptr<Body>& body);
private:
};