#pragma once


#include "IComponent.hpp"
#include "Cooldown.hpp"

#define INPUT_LEN 5


enum MovementState {
    IDLE,
    WALK,
    RUN,
    DODGE_START,
    DODGE,
};

class MovementStateMachine : public IComponent {
protected:
    MovementState currState = IDLE;
    float shiftPressDuration = -1.0;
    Cooldown* dodgeCooldown = new Cooldown(1.0f);
    bool dodgeQueued = false;
    glm::vec2 lastNonZeroInput;

public:
    MovementStateMachine(GameObject* owner) : IComponent(owner) {}
    MovementState Update(float deltaTime, char* input);
    MovementState GetState() { return currState; }
    glm::vec2 GetDodgeDirection() { return lastNonZeroInput; }
    std::string ToString();
};