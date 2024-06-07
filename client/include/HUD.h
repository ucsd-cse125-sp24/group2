#pragma once
#include "TeamInfo.h"
#include "Metronome.h"
#include "../../_common/include/States.hpp"
#include "../../_common/include/IComponent.hpp"
#include "AudioManager.hpp"

class HUDs : public States, public IComponent {
public:
    HUDs();
    HUDs(GameObject* owner);
    void draw(float aspectRatio);
    void triggleText(std::string text);
    void triggleHitText();
    void update(float dt);
    std::string ToString() override;
    void setComboCount(int count);
// private:
    TeamInfo* teamInfo;
    Metronome* metronome;
    HealthBar* healthBar;
    Quad* text;
    std::vector<Quad*> musicNotes;
    HealthBar* bossHealth;
    Quad* hitText;
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