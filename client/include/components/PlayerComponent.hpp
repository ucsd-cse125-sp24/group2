#pragma once

#include "IComponent.hpp"
#include <Health.hpp>

class PlayerComponent : public IComponent {
public:
    bool isLocalPlayer;
    int prevHealth = 100;

    PlayerComponent(GameObject* owner) : IComponent(owner) {
        isLocalPlayer = false;
        prevHealth = owner->GetComponent<Health>()->maxHp;
    }

    void Update(float deltaTime) override;

    std::string ToString() override { return "PlayerComponent"; }
};