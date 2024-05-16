#include "Player.hpp"
#include "Transform.hpp"
#include <iostream>

void Player::update() {
    glm::vec3 input_dir = glm::vec3(inputs.x, 0, -inputs.y);
    velocity =
        (inputs == glm::vec2()) ? glm::vec3() : glm::normalize(input_dir);
    // velocity = glm::vec3();
    if (inputs.y != 0) {
        radius += -inputs.y;
    }
    if (inputs.x != 0) {
        angle += -inputs.x * speed * 0.01;
    }
    glm::vec3 position = glm::vec3(-radius * glm::sin(angle), 0.0f, radius * glm::cos(angle));
    // glm::vec3 position = GetComponent<Transform>()->GetPosition() + velocity * speed;
    GetComponent<Transform>()->SetPosition(position);
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