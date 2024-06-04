#include "EnemyAttack.hpp"
#include "AnimationPlayer.h"
#include "components/RendererComponent.hpp"
#include "AssetManager.hpp"
#include "EnemyComponent.hpp"

#include <iostream>

EnemyAttack::EnemyAttack(Enemy* owner) : Entity() {}

EnemyAttack::EnemyAttack(Enemy* owner, int networkId) : Entity(networkId) {}

EnemyAttack::EnemyAttack(int attackType, int networkId) : Entity(networkId) {
    RendererComponent* meshRenderer =
        new RendererComponent(this, ShaderType::ANIMATED);
    AddComponent(meshRenderer);
    Model* model;
    switch (attackType) {
    case (int) AttackState::SWIPE:
        // model = new Model(AssetManager::Instance().GetModel(path));
        break;
    case (int) AttackState::LASER:
        // model = new Model(AssetManager::Instance().GetModel(path));
        break;
    case (int) AttackState::MARK:
        // model = new Model(AssetManager::Instance().GetModel(path));
        break;
    case (int) AttackState::STOMP:
        // model = new Model(AssetManager::Instance().GetModel(path));
        break;
    }
    AddComponent(model);
    AnimationPlayer* animationPlayer = new AnimationPlayer(this, model);
    AddComponent(animationPlayer);
}