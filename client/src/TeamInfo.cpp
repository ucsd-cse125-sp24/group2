#include "TeamInfo.h"

TeamInfo::TeamInfo() {
    
}

TeamInfo::TeamInfo(int teamSize) {
    // float yOffset = 0.15f;
    // for(int i = 0; i < teamSize; i++) {
    //     teamHealth.push_back(new HealthBar(glm::vec3(-0.82f, 0.75 - yOffset, 0.0f), 0.20f, 0.2f));
    //     yOffset += 0.1f;
    // }
}
TeamInfo::~TeamInfo() {
    auto it = teamHealthMap.begin();
    for(; it != teamHealthMap.end(); it++) {
        delete it->second;
    }
}
void TeamInfo::draw(float aspectRatio) {
    auto it = teamHealthMap.begin();
    for(; it != teamHealthMap.end(); it++) {
        it->second->draw(aspectRatio);
    }
}

void TeamInfo::update() {
    auto it = teamHealthMap.begin();
    for(; it != teamHealthMap.end(); it++) {
        it->second->update();
    }
}

void TeamInfo::addTeamMember(int id) {
    teamHealthMap[id] = new HealthBar(glm::vec3(startPos.x, startPos.y - yOffset, startPos.z), width, height);
    yOffset += 0.1f;
    size++;
    // std::cout<<"Team size: "<<size<<std::endl;
}