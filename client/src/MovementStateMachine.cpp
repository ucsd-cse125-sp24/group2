#include "MovementStateMachine.hpp"


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
        case DEAD_START: {
            result += "DEAD_START";
            break;
        }
        case DEAD: {
            result += "DEAD";
            break;
        }
        default: {
            result += "INVALID";
            break;
        }
    }
    return result;
}