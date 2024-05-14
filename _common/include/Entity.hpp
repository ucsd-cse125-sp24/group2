#ifndef ENTITY_H
#define ENTITY_H
#include "glm/glm.hpp"
#include "NetworkObject.hpp"

class Entity : public NetworkObject {
public:
    Entity() : NetworkObject() {}
    Entity(int networkId) : NetworkObject(networkId) {}
    glm::vec3 position = glm::vec3();
    virtual void update(float deltaTime) = 0;
    void serialize(Packet*) override;
    void deserialize(Packet*) override;
};

#endif // ENTITY_H