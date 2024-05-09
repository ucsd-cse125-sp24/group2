#include "INetworkComponent.hpp"


class Mover : public INetworkComponent {
public:
    float speed;
    glm::vec2 movementHeading;
    glm::vec3& position; // Taken from NetTransform
    glm::vec3& rotation; // Taken from NetTransform

    Mover(NetworkObject* owner);

    void UpdatePhysics();

    virtual void Serialize(Packet*) override;
    virtual void Deserialize(Packet*) override;
};