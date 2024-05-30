#include "engine/Scene.hpp"
#include <algorithm>
void Scene::Update(float deltaTime) {
    for (auto const& entity : entities) {
        entity->update(deltaTime);
        for (int i = 0; i < entity->components.size(); i++) {
            // std::cout << "Updating component: " << entity->components[i]->ToString() << std::endl;
            entity->components[i]->Update(deltaTime);
        }
        for (int i = 0; i < entity->networkComponents.size(); i++) {
            // std::cout << "Updating NetComponent: " << entity->networkComponents[i]->ToString() << std::endl;
            entity->networkComponents[i]->Update(deltaTime);
        }
    }

    for (auto const& gameObject : gameObjects) {
        gameObject->update(deltaTime);
        for (int i = 0; i < gameObject->components.size(); i++) {
            gameObject->components[i]->Update(deltaTime);
        }
    }
}

void Scene::Instantiate(Entity* e) {
    entities.push_back(e);
    ObjectEventArgs* args = new ObjectEventArgs(e);
    object_added.invoke(args);
}

void Scene::Instantiate(GameObject* go) { gameObjects.push_back(go); }

// TODO: Update client states when object is removed
void Scene::Destroy(Entity* e) {
    entities.erase(std::remove(entities.begin(), entities.end(), e),
                   entities.end());
    ObjectEventArgs* args = new ObjectEventArgs(e);
    object_removed.invoke(args);
}

void Scene::Destroy(GameObject* go) {
    gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), go),
                      gameObjects.end());
}