#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <vector>
#include "IComponent.hpp"
class GameObject {
    std::vector<IComponent> components;
};
#endif