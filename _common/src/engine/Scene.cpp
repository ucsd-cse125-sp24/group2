#include "engine/Scene.hpp"
#include <algorithm>
void Scene::Update(float deltaTime) {
    int count = 0;
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
        count++;
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