#include "HUD.h"

HUDs::HUDs() {
    healthBar = new HealthBar(glm::vec3(-0.65f, 0.95f, 0.0f), 0.3f, 0.4f);
    teamInfo = new TeamInfo();
    metronome = new Metronome(60.0f);
    endgame = new Endgame();
}

HUDs::HUDs(GameObject* owner) : IComponent(owner) {
    healthBar = new HealthBar(glm::vec3(-0.65f, 0.95f, 0.0f), 0.3f, 0.04f);
    healthBar->setTexture("empty.png", "../assets/HUD/healthBar");
    healthBar->setTexture("full.png", "../assets/HUD/healthBar");
    teamInfo = new TeamInfo(4);
    metronome = new Metronome(60.0f);
    endgame = new Endgame();
    text = new Quad(glm::vec3(0.0f, 0.1f, 0.0f), 0.2f);
    text->setTexture("perfect.png", "../assets/HUD/text");
    text->setTexture("good.png", "../assets/HUD/text");
    text->setTexture("great.png", "../assets/HUD/text");
    text->setTexture("miss.png", "../assets/HUD/text");
    text->enableState(VISIBLE);
    text->setOpacity(opacity);
    hitText = new Quad(glm::vec3(0.0f, 0.3f, 0.0f), 0.2f);
    hitText->setTexture("hit.png", "../assets/HUD/text");
    hitText->setTexture("heal.png", "../assets/HUD/text");
    hitText->setTexture("revive.png", "../assets/HUD/text");
    hitText->setTexture("speed.png", "../assets/HUD/text");
    hitText->enableState(VISIBLE);
    hitText->setOpacity(opacity);
    float dy = 0.0f;
    for (int i = 0; i < 4; i++) {
        musicNotes.push_back(
            new Quad(glm::vec3(0.9f, -0.5f + dy, 0.0f), 0.09f));
        musicNotes[i]->setTexture("music-note3.png", "../assets/HUD");
        musicNotes[i]->enableState(VISIBLE);
        musicNotes[i]->setOpacity(0.2);
        dy += 0.2f;
    }
    // TODO:: need to get the max health from Boss
    bossHealth = new HealthBar(glm::vec3(-0.10f, -0.8f, 0.0f), 0.5f, 0.04f);
    bossHealth->setMaxHealth(100.0f);
    bossHealth->setHealth(100.0f);
    bossHealth->disableState(VISIBLE);
    // bossHealth->disableState(ObjectStates::VISIBLE);
    bossHealth->setTexture("empty.png", "../assets/HUD/bossHealth");
    bossHealth->setTexture("full.png", "../assets/HUD/bossHealth");

    bearIcon = new Quad(glm::vec3(-0.70f, -0.70f, 0.0f), 0.07f);
    bearIcon->setTexture("boss-icon.png", "../assets/HUD/icon");
    bearIcon->enableState(VISIBLE);

    bearIcon->update();
    text->update();
    hitText->update();
}

void HUDs::draw(float aspectRatio) {
    glEnable(GL_BLEND);
    if (this->checkState(VISIBLE)) {
        healthBar->draw(aspectRatio);
        teamInfo->draw(aspectRatio);
        metronome->draw(aspectRatio);
        text->draw(aspectRatio);
        hitText->draw(aspectRatio);
        bearIcon->draw(aspectRatio);
        endgame->draw(aspectRatio);
        for (int i = 0; i < musicNotes.size(); i++) {
            musicNotes[i]->draw(aspectRatio);
        }
        bossHealth->draw(aspectRatio);
    }
}
float y = 0.1f;
float hitY = 0.1f;
void HUDs::update(float dt) {
    // handle beat correctness text
    if (AudioManager::instance().pressed) {
        triggleText(AudioManager::instance().GetText());
    }

    if (isFade) {
        isFade = true;
        if (opacity <= 0.0f) {
            isFade = false;
        }

        y += 0.001f;
        opacity -= 0.003f;
    }

    text->setOpacity(opacity);
    text->setPosition(glm::vec3(0.0f, y, 0.0f));

    // handle hit text
    if (comboCount == 0) {
        resetNoteOpacity();
    } else if (comboCount == 4) {
        musicNotes[comboCount - 1]->setOpacity(1.0f);
        resetNoteOpacity();
    } else {
        musicNotes[comboCount - 1]->setOpacity(1.0f);
    }

    if (isComboHit) {
        isComboHit = true;
        if (hitOpacity <= 0.0f) {
            isComboHit = false;
        }
        hitOpacity -= 0.002f;
        // hitY += 0.001f;
    }

    // hitText->setPosition(glm::vec3(0.0f, hitY, 0.0f));
    hitText->setOpacity(hitOpacity);

    healthBar->update();
    teamInfo->update();
    metronome->update();
    text->update();
    hitText->update();
    bossHealth->update();
    bearIcon->update();

    for (int i = 0; i < musicNotes.size(); i++) {
        musicNotes[i]->update();
    }
}

std::string HUDs::ToString() { return "HUD"; }

void HUDs::triggleText(std::string text) {
    if (text == "perfect") {
        this->text->activateTexture(0);
    } else if (text == "good") {
        this->text->activateTexture(1);
    } else if (text == "great") {
        this->text->activateTexture(2);
    } else if (text == "offbeat") {
        this->text->activateTexture(3);
    }
    isFade = true;
    y = 0.1f;
    opacity = 1.0f;
}

void HUDs::resetNoteOpacity() {
    for (int i = 0; i < 4; i++) {
        musicNotes[i]->setOpacity(0.2f);
    }
}

void HUDs::setComboCount(int count) { comboCount = count; }

void HUDs::triggleHitText(int comboType) {
    isComboHit = true;
    hitText->activateTexture(comboType <= 2 ? 0 : comboType - 2);
    hitY = 0.1f;
    hitOpacity = 1.0f;
}