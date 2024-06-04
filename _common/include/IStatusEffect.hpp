#pragma once

#include <cstdint>
#include "Status.hpp"

class Status;

enum class StatusEffectTypeID : int32_t {
    // ADD NEW TYPE IDs HERE FOR EACH UNIQUE VARIANT OF A STATUS EFFECT
    DISPLAY_STATUS_EFFECT,
    SPEED_BOOST,
};

class IStatusEffect {
public:
    Status* status;

    bool isActive = false;
    float baseTimer;
    float currTimer = 0.0f;

    bool isStackable;
    int stacks = 1;

    IStatusEffect(Status* status, float baseTimer, bool isStackable);

    void Update(float deltaTime);
    virtual StatusEffectTypeID TypeID() = 0;
    virtual void OnUpdate(float deltaTime) {};
    virtual void OnAdd() {};
    virtual void OnRemove() {};

    void StartTimer();
    void StartTimer(float timeStart);
};