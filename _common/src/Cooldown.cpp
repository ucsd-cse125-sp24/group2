#include "Cooldown.hpp"


Cooldown::Cooldown(float baseCooldown) : baseCooldown(baseCooldown) {}

void Cooldown::StartTimer() { 
    StartTimer(baseCooldown);
}

void Cooldown::StartTimer(float timeStart) {
    onCooldown = true;
    currentTimer = timeStart;
}

void Cooldown::UpdateTimer(float deltaTime) { 
    currentTimer = std::max(currentTimer - deltaTime, 0.0f);
    if (currentTimer == 0.0f) onCooldown = false;
}

bool Cooldown::IsReady() const { return !onCooldown; }

bool Cooldown::UseAbility() {
    if (!onCooldown) {
        StartTimer();
        return true;
    }
    return false;
}