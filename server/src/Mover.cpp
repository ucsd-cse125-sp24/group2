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
    speed = 10.0f;
    // SetCenter(glm::vec2(100.0f, 100.0f));
    glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
    radius = glm::distance(owner->GetComponent<NetTransform>()->position, origin);
    float arccosAngle = glm::acos(glm::dot(owner->GetComponent<NetTransform>()->position - origin, glm::vec3(0.0f, 0.0f, 1.0f)) / radius);
    angle = owner->GetComponent<NetTransform>()->position.x > 0 ? -arccosAngle : arccosAngle;
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
    float oldRadius = radius;
    float oldAngle = angle;
    if (glm::length(input) != 0) {
        input = glm::normalize(input);
    }
    if (input.y != 0) {
        radius += -input.y * speed;
    }
    if (input.x != 0) {
        // angle goes clockwise starting from z axis
        angle += -input.x * speed / radius;
    }

    if (glm::length(input) != 0) {
        // J: feels kind of bad to set nettransform position here directly but works right now ig;
        position =
            center + glm::vec3(-radius * glm::sin(angle), 0.0f, radius * glm::cos(angle));
    }

    // check for collisions
    if (CollisionManager::instance().move(owner, position)) {
        // collision detected
        radius = oldRadius;
        angle = oldAngle;
    }

    float angleAboutY = 180.0f - glm::degrees(angle);
    owner->GetComponent<NetTransform>()->SetRotation(
        glm::vec3(0.0f, angleAboutY, 0.0f));
    owner->GetComponent<Collider>()->SetRotation(
        glm::vec3(0.0f, angleAboutY, 0.0f));
}

void Mover::SetCenter(glm::vec3 newCenter) {
    center = newCenter;
    // TODO: !!! J: we have to reset radius and angle relative to center when it gets moved
    // radius = glm::distance(position, center);
    // angle = glm::acos(glm::dot(position - center, glm::vec3(0.0f, 0.0f, 1.0f)) / radius);
    // if ((position - center).x < 0) angle = -angle;
}

std::string Mover::ToString() { return "Mover"; }