#include "engine/Scene.hpp"
#include <algorithm>
void Scene::Update(float deltaTime) {
    for (auto const& entity : entities) {
        entity->update(deltaTime);
    }
}

void Scene::Instantiate(Entity* e) {
    entities.push_back(e);
    ObjectEventArgs* args = new ObjectEventArgs(e);
    object_added.invoke(args);
}

// TODO: Update client states when object is removed
void Scene::Destroy(Entity* e) {
    entities.erase(std::remove(entities.begin(), entities.end(), e),
                   entities.end());
    ObjectEventArgs* args = new ObjectEventArgs(e);
    object_removed.invoke(args);
}