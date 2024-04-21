#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <vector>
#include "Component.hpp"
#include "Transform.hpp"
#include "core.h"


class GameObject {
    // std::vector<IComponent> components;

public:
    std::vector<IComponent*> components; // should be in private

    GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
    ~GameObject();

    void addComponent(IComponent* comp) { components.push_back(comp); }
};

#endif // GAME_OBJECT_H