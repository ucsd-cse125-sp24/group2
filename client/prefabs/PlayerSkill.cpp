#include "PlayerSkill.hpp"

#include "AnimationPlayer.h"
#include "components/RendererComponent.hpp"
#include "AssetManager.hpp"
#include "PlayerSkillType.hpp"

#include <iostream>

std::string playerAttackPath =
    "../assets/player-note-attack/player-note-attack.gltf";
std::string playerHealPath =
    "../assets/heal-ring/heal-ring.gltf";
std::string playerRevivePath =
    "../assets/revive-ring/revive-ring.gltf";
std::string playerSpeedBoostPath =
    "../assets/speedup-ring/speedup-ring.gltf";

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
    case (int)SkillType::PLAYER_ATTACK:
        GetComponent<NetTransform>()->SetScale(glm::vec3(3.0f));
        meshRenderer = new RendererComponent(this, ShaderType::STANDARD);
        model = new Model(AssetManager::Instance().GetModel(playerAttackPath));
        printf("PlayerAttack!\n");
        break;
    case (int)SkillType::HEAL:
        GetComponent<NetTransform>()->SetScale(glm::vec3(1.0f));
        meshRenderer = new RendererComponent(this, ShaderType::STANDARD);
        model = new Model(AssetManager::Instance().GetModel(playerHealPath));
        printf("PlayerHeal!\n");
        break;
    case (int)SkillType::REVIVE:
        GetComponent<NetTransform>()->SetScale(glm::vec3(1.0f));
        meshRenderer = new RendererComponent(this, ShaderType::STANDARD);
        model = new Model(AssetManager::Instance().GetModel(playerRevivePath));
        printf("PlayerRevive!\n");
        break;
    case (int)SkillType::SPEED_BOOST:
        GetComponent<NetTransform>()->SetScale(glm::vec3(1.0f));
        meshRenderer = new RendererComponent(this, ShaderType::STANDARD);
        model = new Model(AssetManager::Instance().GetModel(playerSpeedBoostPath));
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