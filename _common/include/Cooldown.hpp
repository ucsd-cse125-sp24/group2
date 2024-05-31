#pragma once


#include <algorithm>


class Cooldown {
public:
    bool onCooldown = false;
    float baseCooldown;
    float currentTimer = 0.0f;

    Cooldown(float baseCooldown);

    void StartTimer();
    void StartTimer(float timeStart);
    void UpdateTimer(float deltaTime);
    bool IsReady() const;
    bool UseAbility();
};