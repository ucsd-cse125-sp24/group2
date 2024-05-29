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
    float modelRotation = 0;
    
    if (movementStateMachine) {
        switch(movementStateMachine->GetState()) {
            case(IDLE): {
                animationPlayer->play("idle");
                break;
            }
            case(WALK): {
                // std::cout << "PComp::upd(): Mover::inp: " << glm::to_string(moverInp) << std::endl;
                if (moverDirection.x > 0) {
                    animationPlayer->play("right_strafe_walk");
                } else if (moverDirection.x < 0) {
                    animationPlayer->play("left_strafe_walk");
                } else if (moverDirection.y > 0) {
                    animationPlayer->play("running");
                } else {
                    animationPlayer->play("walk_backward");
                }
                break;
            }
            case(RUN): {
                modelRotation = glm::acos(glm::dot(moverDirection, glm::vec2(0,1)) / glm::length(moverDirection));
                if (moverDirection.x > 0) modelRotation = -modelRotation;
                std::cout << "PComp::upd(): modelRot: " << modelRotation << std::endl;
                animationPlayer->play("running");
                break;
            }
            case(DODGE_START): {
                modelRotation = glm::acos(glm::dot(moverDirection, glm::vec2(0,1)) / glm::length(moverDirection));
                if (moverDirection.x > 0) modelRotation = -modelRotation;
                std::cout << "PComp::upd(): modelRot: " << modelRotation << std::endl;
                animationPlayer->play("forward_roll");
                break;
            }
            case(DODGE): {
                modelRotation = glm::acos(glm::dot(moverDirection, glm::vec2(0,1)) / glm::length(moverDirection));
                if (moverDirection.x > 0) modelRotation = -modelRotation;
                std::cout << "PComp::upd(): modelRot: " << modelRotation << std::endl;
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
    // TODO: rotate model by modelRotation
    // owner->GetComponent<Mover>()->rotation.y += modelRotation; // This don't work
}