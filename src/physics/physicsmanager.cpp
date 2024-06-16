#include "physicsmanager.h"
#include "../log.h"
#include "body.h" // Include here for complete type information

std::vector<std::shared_ptr<Body>> PhysicsManager::activeBodies;

void PhysicsManager::registerBody(const std::shared_ptr<Body>& body) {
    activeBodies.push_back(body);
    Log::console("Body added. Total bodies: " + std::to_string(activeBodies.size()));
}
void PhysicsManager::unregisterBody(const std::shared_ptr<Body>& body) {
    activeBodies.erase(std::remove(activeBodies.begin(), activeBodies.end(), body), activeBodies.end());
    Log::console("Body removed. Total bodies: " + std::to_string(activeBodies.size()));
}