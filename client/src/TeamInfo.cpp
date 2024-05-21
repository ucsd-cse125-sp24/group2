#include "TeamInfo.h"

TeamInfo::TeamInfo() {
    
}

TeamInfo::TeamInfo(int teamSize) {
    float yOffset = 0.15f;
    for(int i = 0; i < teamSize; i++) {
        teamHealth.push_back(new HealthBar(glm::vec3(-0.82f, 0.75 - yOffset, 0.0f), 0.20f, 0.2f));
        yOffset += 0.1f;
    }
}
TeamInfo::~TeamInfo() {
    for(int i = 0; i < teamHealth.size(); i++) {
        delete teamHealth[i];
    }
}
void TeamInfo::draw(float aspectRatio) {
    for(int i = 0; i < teamHealth.size(); i++) {
        teamHealth[i]->draw(aspectRatio);
    }
}

void TeamInfo::update() {
    for(int i = 0; i < teamHealth.size(); i++) {
        teamHealth[i]->update();
    }
}