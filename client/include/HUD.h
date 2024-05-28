#pragma once
#include "TeamInfo.h"
#include "Metronome.h"
#include "../../_common/include/States.hpp"

class HUDs : public States{
public:
    HUDs();
    void draw(float aspectRatio);
    void update(float dt);
// private:
    TeamInfo* teamInfo;
    Metronome* metronome;
    HealthBar* healthBar;
};