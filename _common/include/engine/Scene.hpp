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

    void Instantiate(Entity*);
    void Destroy(Entity*);
    void Update(float deltaTime);
};
#endif