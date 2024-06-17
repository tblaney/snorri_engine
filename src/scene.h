#ifndef SCENE_H
#define SCENE_H

#include "object/object.h"
#include <iostream> 
#include <nlohmann/json.hpp>

class Scene {
public:
    Scene(const std::string& title);
    ~Scene();

    void update();

    void updatePhysics();
    void updateRender();
        
private:
    Object origin;
};

#endif // SCENE_H
