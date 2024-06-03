#pragma once

#include "INetworkComponent.hpp"

class Mover : public INetworkComponent {
public:
    float speed;
    glm::vec2 input;
    glm::vec3 movementHeading;

    glm::vec3& position; // Taken from NetTransform
    glm::vec3& rotation; // Taken from NetTransform

    glm::vec2 center = glm::vec2();
    float radius;
    float angle;

    Mover(NetworkObject* owner);

    void Update();

    virtual void Serialize(Packet* packet) override {
        packet->write_float(speed);
        packet->write_vec3(movementHeading);
    }

    virtual void Deserialize(Packet* packet) override {
        packet->read_float(&speed);
        packet->read_vec3(&movementHeading);
    }

    virtual int32_t TypeID() const override { return MOVER; }

    void SetCenter(glm::vec2 newCenter) { center = newCenter; }

    std::string ToString() override;
};