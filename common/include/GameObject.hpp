#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <vector>
#include "Component.hpp"
#include "../glm/glm.hpp"


class GameObject {

public:
    std::vector<Component> components;

    GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
    
    class Transform : Component {
    public:
        Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

        glm::vec3 pos;
        glm::vec3 rot;
        glm::vec3 scale;
    };
};

#endif // GAME_OBJECT_H