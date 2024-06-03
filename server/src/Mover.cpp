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
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // +y
    float modelRotation = 0;

    if (owner->GetComponent<MovementStateMachine>()) {
        MovementStateMachine* movementStateMachine = owner->GetComponent<MovementStateMachine>();
        movementStateMachine->Update(deltaTime, inputs);

        switch(movementStateMachine->GetState()) {
            case IDLE: {
                UpdatePhysics(deltaTime);
                break;
            }
            case WALK: { // this state is technically not real for mover, but it is for movement state machine.
                speed = baseSpeed;
                UpdatePhysics(deltaTime);
                modelRotation = glm::acos(glm::dot(input, glm::vec2(0,1)) / glm::length(input));
                if (input.x < 0) modelRotation = -modelRotation;
                float angleAboutY = 180.0f - glm::degrees(angle + modelRotation);
                rotation = glm::vec3(0.0f, angleAboutY, 0.0f);
                break;
            }
            case RUN: {
                speed = baseSpeed;
                UpdatePhysics(deltaTime);
                modelRotation = glm::acos(glm::dot(input, glm::vec2(0,1)) / glm::length(input));
                if (input.x < 0) modelRotation = -modelRotation;
                float angleAboutY = 180.0f - glm::degrees(angle + modelRotation);
                rotation = glm::vec3(0.0f, angleAboutY, 0.0f);
                break;
            }
            case DODGE_START: {
                speed = baseSpeed;
                dodgeInput = movementStateMachine->GetDodgeDirection();
                std::cout << "dodgeInput: " << glm::to_string(dodgeInput) << std::endl;
                UpdatePhysics(deltaTime);
                modelRotation = glm::acos(glm::dot(input, glm::vec2(0,1)) / glm::length(input));
                if (input.x < 0) modelRotation = -modelRotation;
                float angleAboutY = 180.0f - glm::degrees(angle + modelRotation);
                rotation = glm::vec3(0.0f, angleAboutY, 0.0f);
                break;
            }
            case DODGE: {
                speed = 1.5 * baseSpeed;
                input = dodgeInput;
                UpdatePhysics(deltaTime);
                modelRotation = glm::acos(glm::dot(input, glm::vec2(0,1)) / glm::length(input));
                if (input.x < 0) modelRotation = -modelRotation;
                float angleAboutY = 180.0f - glm::degrees(angle + modelRotation);
                rotation = glm::vec3(0.0f, angleAboutY, 0.0f);
                break;
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

    // TODO: set changeable center of rotation
    glm::vec3 position =
        glm::vec3(-radius * glm::sin(angle), 0.0f, radius * glm::cos(angle));

    // check for collisions
    if (CollisionManager::instance().move(owner, position)) {
        // collision detected
        radius = oldRadius;
        angle = oldAngle;
    }

    // float angleAboutY = 180.0f - glm::degrees(angle);
    // rotation = glm::vec3(0.0f, angleAboutY, 0.0f);
}

std::string Mover::ToString() { return "Mover"; }