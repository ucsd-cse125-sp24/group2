#include "engine/Scene.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Health.hpp"
#include <algorithm>

void Scene::Update(float deltaTime) {
    for (auto const& entity : entities) {
        entity->update(deltaTime);
        for (int i = 0; i < entity->components.size(); i++) {
            entity->components[i]->Update(deltaTime);
        }
        for (int i = 0; i < entity->networkComponents.size(); i++) {
            entity->networkComponents[i]->Update(deltaTime);
        }
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
    if (Player* p = dynamic_cast<Player*>(e)) {
        p->onDestroy();
    }
    ObjectEventArgs* args = new ObjectEventArgs(e);
    object_removed.invoke(args);
}