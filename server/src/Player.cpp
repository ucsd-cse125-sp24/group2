#include "Player.hpp"
#include "Transform.hpp"
#include <iostream>

void Player::update() {
    glm::vec3 input_dir = glm::vec3(inputs.x, 0, -inputs.y);
    velocity =
        (inputs == glm::vec2()) ? glm::vec3() : glm::normalize(input_dir);
    // velocity = glm::vec3();
    // go forwards and backwards / towards center and away from center
    if (inputs.y != 0) {
        radius += -inputs.y;
    }
    // go left and right / circle around center
    if (inputs.x != 0) {
        angle += -inputs.x * speed * 0.01;
    }
    
    // set position
    glm::vec3 position = glm::vec3(-radius * glm::sin(angle), 0.0f, radius * glm::cos(angle));
    // move in world coordinates
    // glm::vec3 position = GetComponent<Transform>()->GetPosition() + velocity * speed;
    GetComponent<Transform>()->SetPosition(position);

    // set player rotation
    float dotPosWithZ = glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), position);
    float magnitude = glm::length(position);
    float angleAboutY = glm::degrees(glm::acos(dotPosWithZ/magnitude));
    if (position.x >= 0) {
        GetComponent<Transform>()->SetRotation(glm::vec3(90.0f, 180.0f + angleAboutY, 0.0f));
    } else {
        GetComponent<Transform>()->SetRotation(glm::vec3(90.0f, 180.0f - angleAboutY, 0.0f));
    }
    
    // std::cout << glm::to_string(position) << std::endl;
}

int32_t Player::TypeID() const { return NetworkObjectTypeID::PLAYER; }