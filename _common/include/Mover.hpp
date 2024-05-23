#pragma once


#include "INetworkComponent.hpp"
#define INPUT_LEN 5


class Mover : public INetworkComponent {
public:
    float baseSpeed = 5.0f;
    float speed;
    glm::vec2 input;
    glm::vec3 movementHeading;

    char inputs[INPUT_LEN];
    glm::vec2 dodgeInput;

    glm::vec3& position; // Taken from NetTransform
    glm::vec3& rotation; // Taken from NetTransform

    glm::vec2 center = glm::vec2();
    float radius = 500;
    float angle = 0;

    Mover(NetworkObject* owner);

    void Update(float deltaTime);
    void UpdatePhysics(float deltaTime);

    virtual void Serialize(Packet* packet) override {
        packet->write_float(speed);
    }

    virtual void Deserialize(Packet* packet) override {
        packet->read_float(&speed);
    }

    virtual int32_t TypeID() const override { return MOVER; }

    void SetCenter(glm::vec2 newCenter) { center = newCenter; }

    std::string ToString() override;
};