#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <string>
#include <glm/glm.hpp>
#include "IComponent.hpp"

class IComponent;

class GameObject {

protected:
    std::vector<IComponent*> components;
    std::unordered_map<std::type_index, IComponent*> typeToComponentMap;

public:
    GameObject();
    GameObject(glm::vec3 newPosition, glm::vec3 newRotation,
               glm::vec3 newScale);

    virtual std::string ToString();
    virtual void AddComponent(IComponent* newComponent);
    virtual void RemoveComponent(IComponent* comp);

    /**
     * Takes type as "argument" in template, returns IComponent*
     */
    template <class T> T* GetComponent() {
        if (typeToComponentMap.find(typeid(T)) != typeToComponentMap.end()) {
            return static_cast<T*>(typeToComponentMap.at(typeid(T)));
        } else {
            return nullptr;
        } 
    }
};

#endif // GAME_OBJECT_H