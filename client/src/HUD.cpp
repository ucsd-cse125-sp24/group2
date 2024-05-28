#include "HUD.h"

HUDs::HUDs() {
    healthBar = new HealthBar(glm::vec3(-0.60f, 0.98f, 0.0f), 0.45f, 0.3f);
    teamInfo = new TeamInfo(4);
    metronome = new Metronome(60.0f);
}

void HUDs::draw(float aspectRatio) {
    if(this->checkState(VISIBLE)) {
        healthBar->draw(aspectRatio);
        teamInfo->draw(aspectRatio);
        metronome->draw(aspectRatio);
    }
}

void HUDs::update(float dt) {
    healthBar->update();
    teamInfo->update();
    metronome->update(dt);
}