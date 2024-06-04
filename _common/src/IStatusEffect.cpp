#include "IStatusEffect.hpp"
#include <algorithm>


IStatusEffect::IStatusEffect(Status* status, float baseTimer, bool isStackable) :
    status(status),
    baseTimer(baseTimer),
    isStackable(isStackable)
{}

void IStatusEffect::Update(float deltaTime) {
    float deltaTimer = deltaTime;
    if (currTimer - deltaTime <= 0.0f) {
        isActive = false;
        deltaTimer = currTimer;
        currTimer = 0.0f;
    } else {
        currTimer -= deltaTime;
    }

    OnUpdate(deltaTimer);
}

void IStatusEffect::StartTimer() {
    StartTimer(baseTimer);
}

void IStatusEffect::StartTimer(float timeStart) {
    currTimer = timeStart;
    isActive = true;
}