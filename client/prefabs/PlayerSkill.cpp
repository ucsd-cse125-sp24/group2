#include "PlayerSkill.hpp"

#include "AnimationPlayer.h"
#include "components/RendererComponent.hpp"
#include "AssetManager.hpp"
#include "PlayerSkillType.hpp"

#include <iostream>

std::string playerAttackPath = "../assets/player-note-attack/player-note-attack.gltf";

PlayerSkill::PlayerSkill() : Entity() {
    PlayerSkillType* skillType = new PlayerSkillType(this);
    this->AddComponent(skillType);
}

PlayerSkill::PlayerSkill(int networkId) : Entity(networkId) {
    PlayerSkillType* skillType = new PlayerSkillType(this);
    this->AddComponent(skillType);
}

void PlayerSkill::initComponent(int skillType) {
    RendererComponent* meshRenderer = nullptr;
    Model* model = nullptr;
    switch (skillType) {
        case (int) SkillType::PLAYER_ATTACK:
            GetComponent<NetTransform>()->SetScale(glm::vec3(3.0f));
            meshRenderer = new RendererComponent(this, ShaderType::STANDARD);
            model = new Model(AssetManager::Instance().GetModel(playerAttackPath));
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
    if (meshRenderer != nullptr) {
        AddComponent(meshRenderer);
    }
    if (model != nullptr) {
        AddComponent(model);
    }
    // AnimationPlayer* animationPlayer = new AnimationPlayer(this, model);
    // AddComponent(animationPlayer);
}