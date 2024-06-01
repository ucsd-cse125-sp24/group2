#pragma once


#include "IStatusEffect.hpp"
#include <iostream>

class Status;
class IStatusEffect;

class DisplayStatusEffect : public IStatusEffect {
public:
    DisplayStatusEffect(Status* status) : IStatusEffect(status, 0, true) {}

    StatusEffectTypeID TypeID() override { return PLACEHOLDER_STATUS_EFFECT; }
    void OnUpdate(float deltaTime) override {};
    void OnAdd() override {};
    void OnRemove() override {};
};