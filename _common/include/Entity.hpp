#ifndef ENTITY_H
#define ENTITY_H
#include "glm/glm.hpp"
#include "NetworkObject.hpp"

class Entity : public NetworkObject {
public:
    Entity();

    // glm::vec3 position = glm::vec3(); // refactoring to use NetTransform

    virtual void update() = 0;
    // void serialize(Packet*) override; // already implemented in NetworkObject
    // void deserialize(Packet*) override; // already implemented in NetworkObject
};

#endif // ENTITY_H