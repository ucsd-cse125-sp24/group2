#include "HUD.h"

HUDs::HUDs() {
    healthBar = new HealthBar(glm::vec3(-0.70f, 0.95f, 0.0f), 0.45f, 0.3f);
    teamInfo = new TeamInfo();
    metronome = new Metronome(60.0f);
}

HUDs::HUDs(GameObject* owner) : IComponent(owner) {
    healthBar = new HealthBar(glm::vec3(-0.70f, 0.95f, 0.0f), 0.3f, 0.05f);
    teamInfo = new TeamInfo(4);
    metronome = new Metronome(60.0f);
    text = new Quad(glm::vec3(0.0f, 0.1f, 0.0f), 0.2f);
    text->setTexture("perfect.png", "../assets/HUD/text");
    text->setTexture("good.png", "../assets/HUD/text");
    text->setTexture("great.png", "../assets/HUD/text");
    text->setTexture("miss.png", "../assets/HUD/text");
    text->enableState(VISIBLE);
    text->setOpacity(opacity);
    float dy = 0.0f;
    for(int i = 0; i < 4; i++) {
        musicNotes.push_back(new Quad(glm::vec3(0.9f, -0.5f + dy, 0.0f), 0.07f));
        musicNotes[i]->setTexture("music-note2.png", "../assets/HUD");
        musicNotes[i]->enableState(VISIBLE);
        musicNotes[i]->setOpacity(0.2);
        dy += 0.2f;
    }
    text->update();
}

void HUDs::draw(float aspectRatio) {
    glEnable(GL_BLEND);
    if(this->checkState(VISIBLE)) {
        healthBar->draw(aspectRatio);
        teamInfo->draw(aspectRatio);
        metronome->draw(aspectRatio);
        text->draw(aspectRatio);
        for(int i = 0; i < musicNotes.size(); i++) {
            musicNotes[i]->draw(aspectRatio);
        }
    }
}
float y = 0.1f;
void HUDs::update(float dt) {
    time +=dt;
    // std::cout<<"IsFade: "<<isFade<<std::endl;
    if(AudioManager::instance().pressed) {
        triggleText(AudioManager::instance().GetText());
    }

    if(beatHitCount >= 4) {
        resetNoteOpacity();
    }

    if(isFade) {
        isFade = true;
        if(opacity <= 0.0f) {
            isFade = false;
        }
        
        y += 0.002f;
        opacity -= 0.007f;
    }
    // std::cout<<"opacity: "<<opacity<<std::endl;
    text->setOpacity(opacity);
    text->setPosition(glm::vec3(0.0f, y, 0.0f));

    healthBar->update();
    teamInfo->update();
    metronome->update();
    text->update();

    for(int i = 0; i < musicNotes.size(); i++) {
            musicNotes[i]->update();
    }
}

std::string HUDs::ToString() { return "HUD"; }

void HUDs::triggleText(std::string text) {
    if(beatHitCount >= 4) {
        resetNoteOpacity();
    }
    if(text == "perfect") {
        musicNotes[beatHitCount]->setOpacity(1);
        beatHitCount++;
        this->text->activateTexture(0);
    } else if(text == "good") {
        musicNotes[beatHitCount]->setOpacity(1);
        beatHitCount++;
        this->text->activateTexture(1);
    } else if(text == "great") {
        musicNotes[beatHitCount]->setOpacity(1);
        beatHitCount++;
        this->text->activateTexture(2);
    } else if(text == "offbeat") {
        this->text->activateTexture(3);
        resetNoteOpacity();
    }
    isFade = true;
    y = 0.1f;
    opacity = 1.0f;
    time = 0;
}

void HUDs::resetNoteOpacity() {
    for(int i = 0; i < 4; i++) {
        musicNotes[i]->setOpacity(0.2f);
    }
    beatHitCount = 0;
}