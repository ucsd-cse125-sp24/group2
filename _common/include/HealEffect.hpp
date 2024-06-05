#pragma once

#include "IStatusEffect.hpp"
#include <iostream>

class HealEffect : public IStatusEffect {
public:
    HealEffect(Status* status) : IStatusEffect(status, 5.0f, true) {}

    StatusEffectTypeID TypeID() override {
        return StatusEffectTypeID::HEAL;
    }
    void OnUpdate(float deltaTime) {
        status->health->ChangeHealth(5.0f * deltaTime);
    }
    void OnAdd() {
        std::cout << "OnAdd(): Heal " << std::endl;
    }
    void OnRemove() {
        std::cout << "OnRemove(): Heal " << std::endl;
    }
};