#pragma once
#include "TeamInfo.h"
#include "Metronome.h"
#include "Endgame.h"
#include "../../_common/include/States.hpp"
#include "../../_common/include/IComponent.hpp"
#include "AudioManager.hpp"

class HUDs : public States, public IComponent {
public:
    HUDs();
    HUDs(GameObject* owner);
    void draw(float aspectRatio);
    void triggleText(std::string text);
    void triggleHitText(int comboType);
    void update(float dt);
    std::string ToString() override;
    void setComboCount(int count);
    void setComboType(int type);
    // private:
    TeamInfo* teamInfo;
    Metronome* metronome;
    HealthBar* healthBar;
    Endgame* endgame;
    Quad* text;
    std::vector<Quad*> musicNotes;
    HealthBar* bossHealth;
    Quad* attackTypeText;
    Quad* bearIcon;

private:
    void resetNoteOpacity();
    bool isFade = false;
    float time = 0;
    float opacity = 0.0f;
    glm::vec3 textPos;
    int beatHitCount = 0;
    int comboCount = 0;
    float hitOpacity = 0.0f;
    bool isComboHit = false;

};
/*
    playerCombat->AddCombo(attack1, 1);
    playerCombat->AddCombo(attack2, 2);
    playerCombat->AddCombo(heal, 3);
    playerCombat->AddCombo(revive, 4);
    playerCombat->AddCombo(speedBoost, 5);
*/