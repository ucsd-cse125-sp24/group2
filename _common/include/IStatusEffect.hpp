#pragma once


#include <cstdint>
#include "Status.hpp"


class Status;

enum StatusEffectTypeID : int32_t {
    // ADD NEW TYPE IDs HERE FOR EACH UNIQUE VARIANT OF A STATUS EFFECT
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
    virtual void OnUpdate(float deltaTime) = 0;
    virtual void OnAdd() = 0;
    virtual void OnRemove() = 0;

    void StartTimer();
    void StartTimer(float timeStart);
};