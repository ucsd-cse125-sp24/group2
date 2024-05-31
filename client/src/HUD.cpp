#include "HUD.h"

HUDs::HUDs() {
    healthBar = new HealthBar(glm::vec3(-0.60f, 0.98f, 0.0f), 0.45f, 0.3f);
    teamInfo = new TeamInfo();
    metronome = new Metronome(60.0f);
}

HUDs::HUDs(GameObject* owner) : IComponent(owner) {
    healthBar = new HealthBar(glm::vec3(-0.60f, 0.98f, 0.0f), 0.45f, 0.3f);
    teamInfo = new TeamInfo(4);
    metronome = new Metronome(60.0f);
    text = new Quad(glm::vec3(0.0f, 0.1f, 0.0f), 0.2f);
    text->setTexture("perfect.png", "../assets/HUD/text");
    text->enableState(VISIBLE);
    // perfect 0.15 -> 0.2
    text->update();
}

void HUDs::draw(float aspectRatio) {
    glEnable(GL_BLEND);
    if(this->checkState(VISIBLE)) {
        healthBar->draw(aspectRatio);
        teamInfo->draw(aspectRatio);
        metronome->draw(aspectRatio);
        text->draw(aspectRatio);
    }
}

float currentTime = 0;
float opacity = 1.0f;
void HUDs::update(float dt) {
    currentTime +=dt;
    healthBar->update();
    teamInfo->update();
    metronome->update(dt);
    float amount = glm::mix(0.0f, 1.0f, currentTime);
    // std::cout<<"Current Time: "<<currentTime<<std::endl;
    std::cout<<"a: "<<amount<<std::endl;
    // text->setOpacity(opacity - amount);
    text->update();
    // if(currentTime > 2.0f) {
    //     currentTime = 0;
    //     opacity = 1.0f;
    
    // }
}

std::string HUDs::ToString() { return "HUD"; }