#include "Invincible.hpp"

Invincible::Invincible(GameObject* owner) : IComponent(owner) {
    isInvincible = false;
    duration = 0.0f;
}

void Invincible::makeInvincible(float time) {
    isInvincible = true;
    duration = time;
}

void Invincible::update(float deltaTime) {
    if (!isInvincible) {
        return;
    }
    duration -= deltaTime;
    if (duration <= 0) {
        isInvincible = false;
        duration = 0.0f;
    }
}

std::string Invincible::ToString() {
    return "Invincible: " + std::to_string(isInvincible) +
           ", for duration: " + std::to_string(duration);
}