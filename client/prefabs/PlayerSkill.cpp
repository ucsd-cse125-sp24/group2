#include "PlayerSkill.hpp"

#include "AnimationPlayer.h"
#include "components/RendererComponent.hpp"
#include "AssetManager.hpp"
#include "PlayerSkillType.hpp"

#include <iostream>

PlayerSkill::PlayerSkill() : Entity() {
    PlayerSkillType* skillType = new PlayerSkillType(this);
    this->AddComponent(skillType);
}

PlayerSkill::PlayerSkill(int networkId) : Entity(networkId) {
    PlayerSkillType* skillType = new PlayerSkillType(this);
    this->AddComponent(skillType);
}

void PlayerSkill::initComponent(int skillType) {
    // RendererComponent* meshRenderer =
    //     new RendererComponent(this, ShaderType::ANIMATED);
    // AddComponent(meshRenderer);
    // Model* model;
    switch (skillType) {
    case (int) SkillType::PLAYER_ATTACK:
        // model = new Model(AssetManager::Instance().GetModel(path));
        printf("PlayerAttack!\n");
        break;
    case (int) SkillType::HEAL:
        // model = new Model(AssetManager::Instance().GetModel(path));
        printf("PlayerHeal!\n");
        break;
    case (int) SkillType::REVIVE:
        // model = new Model(AssetManager::Instance().GetModel(path));
        printf("PlayerRevive!\n");
        break;
    case (int) SkillType::SPEED_BOOST:
        // model = new Model(AssetManager::Instance().GetModel(path));
        printf("PlayerSpeedBoost!\n");
        break;
    }
    // AddComponent(model);
    // AnimationPlayer* animationPlayer = new AnimationPlayer(this, model);
    // AddComponent(animationPlayer);
}