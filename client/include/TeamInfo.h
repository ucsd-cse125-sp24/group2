#pragma once
#include "core.h"
#include "HealthBar.h"
#include <map>

class TeamInfo : public States{
public:
    TeamInfo();
    TeamInfo(int teamSize);
    ~TeamInfo();
    void draw(float aspectRatio);
    void update();
    void setHealth(int id, float health);
    void addTeamMember(int id);
    std::map<int, HealthBar*> teamHealthMap;
private:
    float yOffset = 0.15f;
    glm::vec3 startPos = glm::vec3(-0.80f, 0.70f, 0.0f);
    float width = 0.15f;
    float height = 0.03f;
    int size = 0;
};
