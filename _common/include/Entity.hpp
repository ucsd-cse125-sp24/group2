#ifndef ENTITY_H
#define ENTITY_H
#include "glm/glm.hpp"
#include "NetworkObject.hpp"

class Entity : public NetworkObject {
public:
    Entity();

    // glm::vec3 position = glm::vec3(); // refactoring to use NetTransform

    virtual void update() = 0;
    void serialize(Packet*) override;
    void deserialize(Packet*) override;
};

#endif // ENTITY_H