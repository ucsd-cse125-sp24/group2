#pragma once


#include "INetworkComponent.hpp"
#include "Cooldown.hpp"

#define INPUT_LEN 5


enum MovementState : int32_t {
    IDLE,
    WALK,
    RUN,
    DODGE_START,
    DODGE,
};

class MovementStateMachine : public INetworkComponent {
protected:
    MovementState currState = IDLE;

    float shiftPressDuration = -1.0;
    Cooldown* dodgeCooldown = new Cooldown(1.0f);
    bool dodgeQueued = false;
    glm::vec2 lastNonZeroInput;

public:
    MovementStateMachine(NetworkObject* owner) : INetworkComponent(owner) {}
    MovementState Update(float deltaTime, char* input);
    MovementState GetState() { return currState; }
    glm::vec2 GetDodgeDirection() { return lastNonZeroInput; }

    void Serialize(Packet* packet) override {
        packet->write_int(currState);
    }

    void Deserialize(Packet* packet) override {
        packet->read_int((int*)&currState);
    }

    int32_t TypeID() const override { return MOVEMENT_STATE_MACHINE; }

    std::string ToString() override;
};