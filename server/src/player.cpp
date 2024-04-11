#include "player.hpp"

void Player::update() {
    position += glm::vec3(inputs.x, 0, inputs.y);
}