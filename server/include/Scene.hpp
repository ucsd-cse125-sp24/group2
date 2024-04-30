#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include "Entity.hpp"
#include "EventArgs.hpp"
#include "Event.hpp"

class Scene {
public:
    EventHandler<ObjectEventArgs> object_removed;
    EventHandler<ObjectEventArgs> object_added;
    std::vector<Entity*> entities;
    void add_object(Entity*);
    void remove_object(Entity*);
    void update();
};
#endif