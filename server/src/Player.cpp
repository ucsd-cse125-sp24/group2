#include "Player.hpp"
#include <iostream>

void Player::update(float deltaTime) {
    glm::vec3 input_dir = glm::vec3(inputs.x, 0, -inputs.y);
    velocity =
        (inputs == glm::vec2()) ? glm::vec3() : glm::normalize(input_dir);
    // velocity = glm::vec3();

    position += velocity * speed;
    // std::cout << glm::to_string(position) << std::endl;
}

int32_t Player::TypeID() const { return NetworkObjectTypeID::PLAYER; }