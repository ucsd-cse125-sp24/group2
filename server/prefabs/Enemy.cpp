#include "prefabs/Enemy.hpp"

float s = 0;
void Enemy::update(float deltaTime) {
    s += deltaTime;
    position += glm::vec3(glm::sin(s), 0, 0);
}