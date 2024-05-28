#pragma once

#include "IComponent.hpp"

class PlayerComponent : public IComponent {
public:
    bool isLocalPlayer;

    PlayerComponent(GameObject* owner) : IComponent(owner) {
      isLocalPlayer = false;
    }

    void Update(float deltaTime) override;

    std::string ToString() override { return "PlayerComponent"; }
};