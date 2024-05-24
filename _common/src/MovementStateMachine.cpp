#include "MovementStateMachine.hpp"
// #include "Mover.hpp"
#include <iostream>


MovementState MovementStateMachine::Update(float deltaTime, char* input) {
    bool FORWARD_PRESSED = (bool)input[0];
    bool LEFT_PRESSED = (bool)input[1];
    bool BACK_PRESSED = (bool)input[2];
    bool RIGHT_PRESSED = (bool)input[3];
    bool LSHIFT_PRESSED = (bool)input[4]; // "SHIFT_PRESSED" name taken by glfw
    bool MOVING_PRESSED = FORWARD_PRESSED != BACK_PRESSED || LEFT_PRESSED != RIGHT_PRESSED;
    dodgeCooldown->UpdateTimer(deltaTime);
    
    switch(currState) {
        case IDLE: {
            if (MOVING_PRESSED) {
                if (LSHIFT_PRESSED) currState = RUN;
                else currState = WALK;
            }
            break;
        }
        case WALK: {
            if (!MOVING_PRESSED) currState = IDLE;
            else if (LSHIFT_PRESSED) currState = RUN;
            break;
        }
        case RUN: {
            if (shiftPressDuration < 0) { // start off shiftPressDuration
                shiftPressDuration = 0;
                // shiftPressDuration += deltaTime; // What if there's lag and there's a big deltaTime? Test for feel
                if (!LSHIFT_PRESSED && dodgeCooldown->IsReady()) {
                    currState = DODGE_START;
                    shiftPressDuration = -1.0f;
                }
            } else { // shiftPressDuration ongoing
                shiftPressDuration += deltaTime;
                if (!LSHIFT_PRESSED) { // let go of shift
                    if (shiftPressDuration <= 0.25f && dodgeCooldown->IsReady()) { // if let go within time limit: dodge
                        currState = DODGE_START;
                    } else if (MOVING_PRESSED) { // otherwise, if still pressing WASD: walk
                        currState = WALK;
                    } else { // otherwise: idle
                        currState = IDLE;
                    }
                    shiftPressDuration = -1.0f;
                } // otherwise, continue running
            }
            break;
        }
        case DODGE_START: {
            dodgeCooldown->UseAbility();
            currState = DODGE;
            break;
        }
        case DODGE: {
            if (dodgeCooldown->IsReady()) {
                if (MOVING_PRESSED) {
                    if (LSHIFT_PRESSED) currState = RUN;
                    else currState = WALK;
                } else {
                    currState = IDLE;
                }
            }
            break;
        }
        default: {
            std::cout << "INVALID MOVEMENT STATE" << std::endl;
        }
    }
    return currState;
}

std::string MovementStateMachine::ToString() {
    std::string result = "MovementStateMachine; currState: ";
    switch (currState) {
        case IDLE: {
            result += "IDLE";
            break;
        }
        case WALK: {
            result += "WALK";
            break;
        }
        case RUN: {
            result += "RUN";
            break;
        }
        case DODGE_START: {
            result += "DODGE_START";
            break;
        }
        case DODGE: {
            result += "DODGE";
            break;
        }
        default: {
            result += "INVALID";
            break;
        }
    }
    return result;
}