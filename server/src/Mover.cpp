#include "Mover.hpp"
#include "NetTransform.hpp"
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "CollisionManager.hpp"
#include "MovementStateMachine.hpp"
#include "Health.hpp"

Mover::Mover(NetworkObject* owner)
    : INetworkComponent(owner),
      position(owner->GetComponent<NetTransform>()->position),
      rotation(owner->GetComponent<NetTransform>()->rotation) {
    // TODO: handle multiple speeds
    speed = 1.0f;
} 

void Mover::Update(float deltaTime) {
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // +y
    float modelRotation = 0;

    if (owner->GetComponent<MovementStateMachine>()) {
        MovementStateMachine* movementStateMachine = owner->GetComponent<MovementStateMachine>();

        // probably very bad, because it was decoupled and now i just coupled it :P
        // TODO: pls help me fix this tim T-T
        if (owner->GetComponent<Health>()->GetHealth() <= 0) {
            movementStateMachine->SetState(MovementState::DEAD_START);
        }

        movementStateMachine->Update(deltaTime, inputs);

        UpdatePhysics(deltaTime);
        modelRotation = glm::degrees(glm::acos(glm::dot(input, glm::vec2(0,1))));
        if (input.x > 0) modelRotation = -modelRotation;
        rotation += glm::vec3(0.0f, modelRotation, 0.0f);

        switch(movementStateMachine->GetState()) {
            case IDLE: {
                UpdatePhysics(deltaTime);
                break;
            }
            case WALK: { // this state is technically not real for mover, but it is for movement state machine.
                speed = baseSpeed;
                break;
            }
            case RUN: {
                speed = 2 * baseSpeed;
                break;
            }
            case DODGE_START: {
                speed = baseSpeed;
                dodgeInput = movementStateMachine->GetDodgeDirection();
                std::cout << "dodgeInput: " << glm::to_string(dodgeInput) << std::endl;
                break;
            }
            case DODGE: {
                speed = 1.5 * baseSpeed;
                input = dodgeInput;
                break;
            }
            case DEAD: {
                speed = 0;
                input = glm::vec2(0.0f);
                // UpdatePhysics(deltaTime);
                // use old modelRotation and rotation i think
            }
            default: {
                break;
            }
        }
    } else {
        UpdatePhysics(deltaTime);
    }
}

void Mover::UpdatePhysics(float deltaTime) {
    glm::vec3 newPosition = position;
    glm::vec3 direction = glm::normalize(center - position);
    if (glm::length(input) != 0) {
        input = glm::normalize(input);
        // this really relies on y = 0
        glm::vec3 right = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
        newPosition += direction * input.y * speed + right * input.x * speed;
    }

    // check for collisions
    if (CollisionManager::instance().move(owner, newPosition)) {
        // collision detected
    }

    // both direction and z unit vector have length 1
    float angleAboutY = glm::degrees(glm::acos(glm::dot(direction, glm::vec3(0.0f, 0.0f, 1.0f))));
    if (direction.x < 0) angleAboutY = -angleAboutY;
    owner->GetComponent<NetTransform>()->SetRotation(
        glm::vec3(0.0f, angleAboutY, 0.0f));
    owner->GetComponent<Collider>()->SetRotation(
        glm::vec3(0.0f, angleAboutY, 0.0f));
}

void Mover::SetCenter(glm::vec3 newCenter) {
    center = newCenter;
}

std::string Mover::ToString() { return "Mover"; }