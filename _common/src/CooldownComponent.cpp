#include "CooldownComponent.hpp"


CooldownComponent::~CooldownComponent() {
    for (auto kvPair : cooldowns) {
        Cooldown* currCooldown = kvPair.second;
        delete currCooldown;
    }
}

void CooldownComponent::AddCooldown(std::string name, float baseCd) {
    cooldowns.emplace(name, new Cooldown(baseCd));
}

void CooldownComponent::RemoveCooldown(std::string name) {
    cooldowns.erase(name);
}

bool CooldownComponent::UseAbility(std::string name) {
    if (cooldowns.find(name) != cooldowns.end() && cooldowns[name]->IsReady()) {
        cooldowns[name]->StartTimer();
        return true;
    }
    return false;
}

void CooldownComponent::Update(float deltaTime) {
    for (auto kvPair : cooldowns) {
        Cooldown* currCooldown = kvPair.second;
        if (currCooldown->onCooldown) {
            currCooldown->UpdateTimer(deltaTime);
        }
    }
}

bool CooldownComponent::IsReady(std::string name) {
    if (cooldowns.find(name) != cooldowns.end()) {
        return cooldowns[name]->IsReady();
    }
    return false;
}