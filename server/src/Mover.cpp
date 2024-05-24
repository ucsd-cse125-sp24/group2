#include "Mover.hpp"
#include "NetTransform.hpp"
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "CollisionManager.hpp"
#include "MovementStateMachine.hpp"

Mover::Mover(NetworkObject* owner)
    : INetworkComponent(owner),
      position(owner->GetComponent<NetTransform>()->position),
      rotation(owner->GetComponent<NetTransform>()->rotation) {
    // TODO: handle multiple speeds
    speed = 10.0f;
    // SetCenter(glm::vec2(100.0f, 100.0f));
}

void Mover::Update(float deltaTime) {
    if (glm::length(input) != 0) {
        lastNonZeroInput = input;
    }
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // +y

    if (owner->GetComponent<MovementStateMachine>()) {
        MovementStateMachine* movementStateMachine = owner->GetComponent<MovementStateMachine>();
        movementStateMachine->Update(deltaTime, inputs);

        std::cout << movementStateMachine->ToString() << std::endl;

        switch(movementStateMachine->GetState()) {
            case IDLE: {
                // do nothing
                UpdatePhysics(deltaTime);
                break;
            }
            case WALK: {
                speed = baseSpeed;
                UpdatePhysics(deltaTime);
                break;
            }
            case RUN: {
                speed = 2.0f * baseSpeed;
                UpdatePhysics(deltaTime);
                break;
            }
            case DODGE_START: {
                speed = 2.0f * baseSpeed;
                // dodgeInput = input;
                std::cout << "input: " << glm::to_string(input) << std::endl;
                UpdatePhysics(deltaTime);
                break;
            }
            case DODGE: {
                speed = 2.5f * baseSpeed;
                input = dodgeInput;
                UpdatePhysics(deltaTime);
                break;
            }
            default: {
                break;
            }
        }
    } else {
        // std::cout << "No MovementSM" << std::endl;
        UpdatePhysics(deltaTime);
    }
}

void Mover::UpdatePhysics(float deltaTime) {
    float oldRadius = radius;
    float oldAngle = angle;
    if (glm::length(input) != 0) {
        input = glm::normalize(input);
    }
    if (input.y != 0) {
        radius += -input.y * speed;
    }
    if (input.x != 0) {
        angle += -input.x * speed / radius;
    }

    glm::vec3 position =
        glm::vec3(-radius * glm::sin(angle), 0.0f, radius * glm::cos(angle));

    // check for collisions
    if (CollisionManager::instance().move(owner, position)) {
        // collision detected
        radius = oldRadius;
        angle = oldAngle;
    }

    owner->GetComponent<NetTransform>()->SetPosition(owner->GetComponent<Transform>()->GetPosition());

    float angleAboutY = 180.0f-glm::degrees(angle);
    owner->GetComponent<NetTransform>()->SetRotation(glm::vec3(0.0f, angleAboutY, 0.0f));
}

std::string Mover::ToString() { return "Mover"; }