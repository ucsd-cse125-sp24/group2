#ifndef I_COMPONENT_H
#define I_COMPONENT_H

#include "GameObject.hpp"
#include <string>


class GameObject;


class IComponent {

protected:
    GameObject* owner;

public:
    IComponent() : owner(nullptr) {}
    IComponent(GameObject* newOwner) : owner(newOwner) {}
    void SetOwner(GameObject* newOwner) {owner = newOwner;}

    virtual std::string ToString() = 0; //might not work as pure virtual function
};


#endif // I_COMPONENT_H
