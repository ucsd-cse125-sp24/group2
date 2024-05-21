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
}

void Mover::Update() {
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // +y

    float oldRadius = radius;
    float oldAngle = angle;
    if (input.y != 0) {
        radius += -input.y * speed;
    }
    if (input.x != 0) {
        angle += -input.x * speed / radius;
    }

    float angleDeg = glm::degrees(angle);

    std::cout << "angle: " << angle;
    std::cout << " | angleDeg: " << angleDeg;
    std::cout << " | input.x: " << input.x;
    std::cout << " | speed: " << speed;
    std::cout << " | radius: " << radius;

    glm::vec3 position =
        glm::vec3(-radius * glm::sin(angle), 0.0f, radius * glm::cos(angle));

    // check for collisions
    if (CollisionManager::instance().move(owner, position)) {
        // collision detected
        std::cout << "\nCOLLIDE!" << std::endl;
        radius = oldRadius;
        angle = oldAngle;
    }

    owner->GetComponent<NetTransform>()->SetPosition(owner->GetComponent<Transform>()->GetPosition());

    float angleAboutY = 180.0f-glm::degrees(angle);
    std::cout << " | abtY: " << angleAboutY << std::endl;

    owner->GetComponent<NetTransform>()->SetRotation(glm::vec3(0.0f, angleAboutY, 0.0f));
}

std::string Mover::ToString() { return "Mover"; }