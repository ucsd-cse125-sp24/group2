#include "components/PlayerComponent.hpp"
#include <InputManager.h>
#include <AnimationPlayer.h>
#include <Mover.hpp>
#include <NetTransform.hpp>
#include <Health.hpp>

#include <MovementStateMachine.hpp>

void PlayerComponent::Update(float deltaTime) {
    MovementStateMachine* movementStateMachine =
        owner->GetComponent<MovementStateMachine>();
    AnimationPlayer* animationPlayer = owner->GetComponent<AnimationPlayer>();
    glm::vec2 moverDirection = owner->GetComponent<Mover>()->input;

    if (movementStateMachine) {
        switch(movementStateMachine->GetState()) {
            case(IDLE): {
                animationPlayer->play("idle", false);
                break;
            }
            case(WALK): {
                animationPlayer->play("running", false);
                break;
            }
            case(RUN): {
                animationPlayer->play("running", false);
                break;
            }
            case(DODGE_START): {
                animationPlayer->play("forward_roll", true);
                break;
            }
            case(DODGE): {
                animationPlayer->play("forward_roll", false);
                break;
            }
            case(DEAD_START): {
                animationPlayer->play("dying", false);
                break;
            }
            case(DEAD): {
                animationPlayer->play("dead", false);
                break;
            }
            default: {
                animationPlayer->play("idle", false);
            }
        }
    } else {
        animationPlayer->play("idle", false);
    }
}