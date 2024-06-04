#pragma once


#include "INetworkComponent.hpp"
#define INPUT_LEN 5


class Mover : public INetworkComponent {
public:
    float baseSpeed = 10.0f;
    float speed;
    glm::vec2 input;
    glm::vec3 movementHeading;

    char inputs[INPUT_LEN];
    glm::vec2 dodgeInput;

    glm::vec3& position; // Taken from NetTransform
    glm::vec3& rotation; // Taken from NetTransform

    glm::vec3 center = glm::vec3();
    float radius;
    float angle;

    Mover(NetworkObject* owner);

    void Update(float deltaTime);
    void UpdatePhysics(float deltaTime);

    virtual void Serialize(Packet* packet) override {
        packet->write_float(speed);
        packet->write_float(input.x);
        packet->write_float(input.y);
    }

    virtual void Deserialize(Packet* packet) override {
        packet->read_float(&speed);
        packet->read_float(&input.x);
        packet->read_float(&input.y);
    }

    virtual int32_t TypeID() const override { return MOVER; }

    void SetCenter(glm::vec3 newCenter);
    std::string ToString() override;
};