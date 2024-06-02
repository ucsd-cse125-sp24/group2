#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include "Entity.hpp"
#include "EventArgs.hpp"
#include "Event.hpp"

class Scene {
public:
    Scene() {}

    EventHandler<ObjectEventArgs> object_removed;
    EventHandler<ObjectEventArgs> object_added;
    std::vector<Entity*> entities;
    std::vector<GameObject*> gameObjects;

    void Instantiate(Entity*);
    void Instantiate(GameObject*);
    void Destroy(Entity*);
    void Destroy(GameObject*);
    void Update(float deltaTime);
};
#endif