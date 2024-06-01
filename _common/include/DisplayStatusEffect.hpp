#pragma once


#include "IStatusEffect.hpp"
#include <iostream>

class Status;
class IStatusEffect;

class DisplayStatusEffect : public IStatusEffect {
public:
    DisplayStatusEffect(Status* status) : IStatusEffect(status, 0, true) {}

    StatusEffectTypeID TypeID() override { return DISPLAY_STATUS_EFFECT; }
};