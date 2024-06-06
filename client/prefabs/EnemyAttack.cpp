#include "EnemyAttack.hpp"
#include "AnimationPlayer.h"
#include "components/RendererComponent.hpp"
#include "AssetManager.hpp"
#include "EnemyComponent.hpp"

#include <iostream>

// note: these are model paths relative to main.cpp
// as that is where AssetManager initializes its modelpath list
std::string swipePath = "";
std::string laserPath = "../assets/laser-attack/gltf-beam/laser-beam-model.gltf";
std::string markPath = "../assets/projectile-attack/projectile-attack-manual.gltf";
std::string stompPath = "../assets/wave-attack/WaveAttack.gltf";

EnemyAttack::EnemyAttack(Enemy* owner) : Entity() {}

EnemyAttack::EnemyAttack(Enemy* owner, int networkId) : Entity(networkId) {}

EnemyAttack::EnemyAttack(int attackType, int networkId) : Entity(networkId) {
    this->attackType = attackType;
    RendererComponent* meshRenderer =
        new RendererComponent(this, ShaderType::ANIMATED);
    AddComponent(meshRenderer);
    Model* model;
    switch (attackType) {
    case (int) AttackState::SWIPE:
        model = new Model(AssetManager::Instance().GetModel(swipePath));
        printf("SwipeAttack!\n");
        break;
    case (int) AttackState::LASER:
        model = new Model(AssetManager::Instance().GetModel(laserPath));
        printf("LaserAttack!\n");
        break;
    case (int) AttackState::MARK:
        model = new Model(AssetManager::Instance().GetModel(markPath));
        printf("MarkedAttack!\n");
        break;
    case (int) AttackState::STOMP:
        model = new Model(AssetManager::Instance().GetModel(stompPath));
        printf("StompAttack!\n");
        break;
    }
    AddComponent(model);
    AnimationPlayer* animationPlayer = new AnimationPlayer(this, model);
    AddComponent(animationPlayer);
}

void EnemyAttack::update(float deltaTime) {
    switch (attackType) {
        case (int) AttackState::SWIPE:
            break;
        case (int) AttackState::LASER:
            GetComponent<AnimationPlayer>()->play("laser-beamAction");
            break;
        case (int) AttackState::MARK:
            GetComponent<AnimationPlayer>()->play("Key.001Action.002");
            break;
        case (int) AttackState::STOMP:
            GetComponent<AnimationPlayer>()->play("WaveAttack");
            break;
    }
}