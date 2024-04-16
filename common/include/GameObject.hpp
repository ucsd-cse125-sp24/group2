#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <vector>
#include "Component.hpp"
#include "../glm/glm.hpp"


class GameObject {
    std::vector<Component> components;

public:
    GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

    void addComponent(Component comp) { components.push_back(comp); }
};

#endif // GAME_OBJECT_H