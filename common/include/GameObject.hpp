#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <vector>
#include "Component.hpp"
#include "Transform.hpp"
#include "../glm/glm.hpp"


class GameObject {
    std::vector<IComponent> components;

public:
    GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

    void addComponent(IComponent comp) { components.push_back(comp); }
};


// FUNCTION DEFINITIONS

GameObject::GameObject(glm::vec3 pos = glm::vec3(), glm::vec3 rot = glm::vec3(), glm::vec3 scale = glm::vec3()) {
    Transform transform = Transform(pos, rot, scale);
    addComponent(transform);
}


#endif // GAME_OBJECT_H