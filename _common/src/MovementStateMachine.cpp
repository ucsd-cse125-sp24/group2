#include "MovementStateMachine.hpp"


MovementState MovementStateMachine::Update(float deltaTime, char input[INPUT_LEN]) {
    bool FORWARD_PRESSED = (bool)input[0];
    bool LEFT_PRESSED = (bool)input[1];
    bool BACK_PRESSED = (bool)input[2];
    bool RIGHT_PRESSED = (bool)input[3];
    bool SHIFT_PRESSED = (bool)input[4];
    bool ANY_WASD_PRESSED = FORWARD_PRESSED || LEFT_PRESSED || BACK_PRESSED || RIGHT_PRESSED;
    switch(currState) {
        case IDLE: {
            if (ANY_WASD_PRESSED) {
                if (SHIFT_PRESSED) currState = RUN;
                else currState = WALK;
            }
            break;
        }
        case WALK: {
            if (!ANY_WASD_PRESSED) currState = IDLE;
            else if (SHIFT_PRESSED) currState = RUN;
            break;
        }
        case RUN: {
            if (shiftPressDuration < 0) {
                shiftPressDuration = 0;
                // shiftPressDuration += deltaTime; // What if there's lag and there's a big deltaTime? Test for feel
                if (!SHIFT_PRESSED) currState = DODGE;
            } else {
                shiftPressDuration += deltaTime;
                if (!SHIFT_PRESSED && shiftPressDuration <= 0.25) currState = DODGE;
                else if (ANY_WASD_PRESSED) currState = WALK;
                else currState = IDLE;
            }
            break;
        }
        case DODGE: {
            break;
        }
        default: {

        }
    }
}