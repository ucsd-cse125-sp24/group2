#include "components/PlayerComponent.hpp"
#include <InputManager.h>
#include <AnimationPlayer.h>
#include <Mover.hpp>
#include <NetTransform.hpp>
#include <Health.hpp>

// void PlayerComponent::Update(float deltaTime) {
//     if (owner->GetComponent<Health>()->GetHealth() <= 0) {
//         // TODO: add animation for death
//         // TODO: stop player movement, probably just set speed in mover.
//         owner->GetComponent<NetTransform>()->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
//         //owner->GetComponent<AnimationPlayer>()->play("dead");
//         owner->GetComponent<AnimationPlayer>()->play("idle");
//     } else if (glm::length(owner->GetComponent<Mover>()->movementHeading) > 0) {
//         owner->GetComponent<AnimationPlayer>()->play("run");
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
            // case(DEAD): {
            //     animationPlayer->play("dying");
            //     break;
            // }
            default: {
                animationPlayer->play("idle");
            }
        }
    } else {
        animationPlayer->play("idle");
    }
}