#pragma once

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

    virtual void Serialize(Packet* packet) override {
        packet->write_float(speed);
    }

    virtual void Deserialize(Packet* packet) override {
        packet->read_float(&speed);
    }

    virtual int32_t TypeID() const override { return MOVER; }

    std::string ToString() override;
};