#include "Mover.hpp"
#include "NetTransform.hpp"
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "CollisionManager.hpp"

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

void Mover::Update() {
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // +y

    movementHeading = glm::vec3(input.x, 0, input.y);

    float oldRadius = radius;
    float oldAngle = angle;
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

    float angleAboutY = 180.0f - glm::degrees(angle);
    owner->GetComponent<NetTransform>()->SetRotation(
        glm::vec3(0.0f, angleAboutY, 0.0f));
    owner->GetComponent<Collider>()->SetRotation(
        glm::vec3(0.0f, angleAboutY, 0.0f));
}

std::string Mover::ToString() { return "Mover"; }