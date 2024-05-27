#ifndef ENTITY_H
#define ENTITY_H
#include "glm/glm.hpp"
#include "NetworkObject.hpp"

class Entity : public NetworkObject {
public:
    Entity();
    Entity(int networkId);
    virtual void update(float deltaTime) = 0;
};

#endif // ENTITY_H