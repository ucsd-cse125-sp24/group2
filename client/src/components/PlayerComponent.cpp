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
            case(DEAD_START): {
                animationPlayer->play("dying");
                break;
            }
            case(DEAD): {
                animationPlayer->play("idle");
                owner->GetComponent<NetTransform>()->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
                // TODO: also need a dead kind of idle animation;
                // we actually need to states for dead like dead_start and dead
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