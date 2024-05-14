#include "Mover.hpp"
#include "NetTransform.hpp"
#include <iostream>
#include "glm/gtx/string_cast.hpp"

Mover::Mover(NetworkObject* owner)
    : INetworkComponent(owner),
      position(owner->GetComponent<NetTransform>()->position),
      rotation(owner->GetComponent<NetTransform>()->rotation) {
    // TODO: handle multiple speeds
    speed = 0.2f;
}

void Mover::Update() {
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // +y
    glm::mat4 rotate = glm::mat4(1.0f);
    rotate = glm::rotate(rotate, rotation.y, up);

    movementHeading = glm::vec3(input.x, 0, input.y);

    if (movementHeading != glm::vec3())
        movementHeading = glm::normalize(movementHeading);

    position += glm::vec3(rotate * glm::vec4(movementHeading, 0) * speed);
}

std::string Mover::ToString() { return "Mover"; }