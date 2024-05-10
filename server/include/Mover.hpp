#include "INetworkComponent.hpp"


class Mover : public INetworkComponent {
public:
    float speed;
    glm::vec2 input;
    glm::vec3 movementHeading;

    glm::vec3& position; // Taken from NetTransform
    glm::vec3& rotation; // Taken from NetTransform

    Mover(NetworkObject* owner);

    void Update();

    virtual void Serialize(Packet*) override;
    virtual void Deserialize(Packet*) override;

    std::string ToString() override;
};