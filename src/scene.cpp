#include "scene.h"
#include "file/pathutils.h"
#include "file/jsonloader.h"

Scene::Scene(const std::string& title) 
    : origin(JsonLoader::loadJsonFile(getAssetPath(title).string())) {
}

void Scene::update() {
    origin.update();
}
void Scene::updatePhysics() {
    origin.updatePhysics();
}
void Scene::updateRender() {
    origin.updateRender();
}
Scene::~Scene() {
}