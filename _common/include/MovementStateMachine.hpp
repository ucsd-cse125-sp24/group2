#pragma once


#include "IComponent.hpp"

#define INPUT_LEN 5


enum MovementState {
    IDLE,
    WALK,
    RUN,
    DODGE,
};

class MovementStateMachine : public IComponent {
protected:
    MovementState currState = IDLE;
    float shiftPressDuration = -1.0;
    float dodgeTime = 1.0; // maybe use Cooldown here instead?

public:
    MovementState Update(float deltaTime, char input[INPUT_LEN]);
};