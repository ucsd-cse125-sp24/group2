#include "components/PlayerComponent.hpp"
#include <InputManager.h>
#include <AnimationPlayer.h>
#include <Mover.hpp>
#include <NetTransform.hpp>
#include <MovementStateMachine.hpp>

void PlayerComponent::Update(float deltaTime) {
    MovementStateMachine* movementStateMachine = owner->GetComponent<MovementStateMachine>();
    AnimationPlayer* animationPlayer = owner->GetComponent<AnimationPlayer>();
    glm::vec2 moverDirection = owner->GetComponent<Mover>()->input;
    
    if (movementStateMachine) {
        switch(movementStateMachine->GetState()) {
            case(IDLE): {
                animationPlayer->play("idle");
                break;
            }
            case(WALK): {
                animationPlayer->play("running");
                break;
            }
            case(RUN): {
                animationPlayer->play("running");
                break;
            }
            case(DODGE_START): {
                animationPlayer->play("forward_roll");
                break;
            }
            case(DODGE): {
                animationPlayer->play("forward_roll");
                break;
            }
            default: {
                animationPlayer->play("idle");
            }
        }
    } else {
        animationPlayer->play("idle");
    }
}