#pragma once
#include "core.h"
#include "HealthBar.h"

class TeamInfo {
public:
    TeamInfo();
    TeamInfo(int teamSize);
    ~TeamInfo();
    void draw(float aspectRatio);
    void update();
private:
    std::vector<HealthBar*> teamHealth;
};