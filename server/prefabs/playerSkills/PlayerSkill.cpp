#include "PlayerSkill.hpp"

PlayerSkill::PlayerSkill() : Entity() {
    exist = true;
    PlayerSkillType* skillType = new PlayerSkillType(this);
    this->AddComponent(skillType);
}
PlayerSkill::PlayerSkill(int networkId) : Entity(networkId) {
    exist = true;
    PlayerSkillType* skillType = new PlayerSkillType(this);
    this->AddComponent(skillType);
}