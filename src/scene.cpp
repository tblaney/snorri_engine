#include "scene.h"
#include "file/pathutils.h"
#include "file/jsonloader.h"

Scene::Scene(const std::string& title) 
    : origin(JsonLoader::loadJsonFile(getAssetPath(title).string())) {
}

void Scene::update() {
    origin.update();
}

Scene::~Scene() {
}