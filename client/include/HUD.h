#pragma once
#include "TeamInfo.h"
#include "Metronome.h"

class HUDs {
public:
    HUDs();
    void draw(float aspectRatio);
    void update(float dt);
private:
    TeamInfo* teamInfo;
    Metronome* metronome;
    HealthBar* healthBar;
};