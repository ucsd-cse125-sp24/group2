#include "prefabs/Enemy.hpp"
#include "NetTransform.hpp"

float s = 0;
void Enemy::update(float deltaTime) {
    s += deltaTime;
    // position += glm::vec3(glm::sin(s), 0, 0);
    GetComponent<NetTransform>()->position += glm::vec3(glm::sin(s), 0, 0);
}