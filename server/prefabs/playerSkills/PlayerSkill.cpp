#include "PlayerSkill.hpp"

PlayerSkill::PlayerSkill() : Entity() {
    PlayerSkillType* skillType = new PlayerSkillType(this);
    this->AddComponent(skillType);
}
PlayerSkill::PlayerSkill(int networkId) : Entity(networkId) {
    PlayerSkillType* skillType = new PlayerSkillType(this);
    this->AddComponent(skillType);
}