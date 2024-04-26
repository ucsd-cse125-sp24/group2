#include "Scene.hpp"
#include <algorithm>
void Scene::update() {
    for (auto const& entity : entities) {
        entity->update();
    }
}

void Scene::add_object(Entity* e) { entities.push_back(e); }

void Scene::remove_object(Entity* e) {
    entities.erase(std::remove(entities.begin(), entities.end(), e),
                   entities.end());
}