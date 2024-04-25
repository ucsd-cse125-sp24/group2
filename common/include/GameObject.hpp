#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H


#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <string>
#include "../glm/glm.hpp"
#include "IComponent.hpp"


class IComponent;


class GameObject {
    std::vector<IComponent*> components;
    std::unordered_map<std::type_index, IComponent*> typeToComponentMap;

public:
    GameObject();
    GameObject(glm::vec3 newPosition, glm::vec3 newRotation, glm::vec3 newScale);

    virtual std::string toString();
    IComponent* getComponent(std::type_index type);
    void addComponent(IComponent* newComponent);
};


#endif // GAME_OBJECT_H