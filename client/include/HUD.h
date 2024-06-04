#pragma once
#include "TeamInfo.h"
#include "Metronome.h"
#include "../../_common/include/States.hpp"
#include "../../_common/include/IComponent.hpp"

class HUDs : public States, public IComponent {
public:
    HUDs();
    HUDs(GameObject* owner);
    void draw(float aspectRatio);
    void update(float dt);
    std::string ToString() override;
// private:
    TeamInfo* teamInfo;
    Metronome* metronome;
    HealthBar* healthBar;
};