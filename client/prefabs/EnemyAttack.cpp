#include "EnemyAttack.hpp"
#include "AnimationPlayer.h"
#include "components/RendererComponent.hpp"
#include "AssetManager.hpp"
#include "EnemyComponent.hpp"

#include <iostream>

// note: these are model paths relative to main.cpp
// as that is where AssetManager initializes its modelpath list
std::string swipePath = "../assets/swipe-attack-v2/swipe-attack.gltf";
std::string laserPath = "../assets/laser-attack/gltf-beam/laser-beam-model.gltf";
std::string markPath = "../assets/projectile-attack/projectile-attack-manual.gltf";
std::string stompPath = "../assets/wave-attack-v3/WaveAttack.gltf";

EnemyAttack::EnemyAttack(Enemy* owner) : Entity() {}

EnemyAttack::EnemyAttack(Enemy* owner, int networkId) : Entity(networkId) {}

EnemyAttack::EnemyAttack(int attackType, int networkId) : Entity(networkId) {
    this->attackType = attackType;
    RendererComponent* meshRenderer;
    Model* model;
    std::string path;
    switch (attackType) {
        case (int) AttackState::SWIPE:
            meshRenderer = new RendererComponent(this, ShaderType::STANDARD);
            GetComponent<NetTransform>()->SetScale(glm::vec3(100.0f));
            model = new Model(AssetManager::Instance().GetModel(swipePath));
            path = swipePath;
            printf("SwipeAttack!\n");
            break;
        case (int) AttackState::LASER:
            meshRenderer = new RendererComponent(this, ShaderType::ANIMATED);
            // model = new Model(AssetManager::Instance().GetModel(laserPath));
            path = laserPath;
            printf("LaserAttack!\n");
            break;
        case (int) AttackState::MARK:
            meshRenderer = new RendererComponent(this, ShaderType::ANIMATED);
            // model = new Model(AssetManager::Instance().GetModel(markPath));
            path = markPath;
            printf("MarkedAttack!\n");
            break;
        case (int) AttackState::STOMP:
            meshRenderer = new RendererComponent(this, ShaderType::ANIMATED);
            model = new Model(AssetManager::Instance().GetModel(stompPath));
            path = stompPath;
            printf("StompAttack!\n");
            break;
    }
    AddComponent(meshRenderer);
    AddComponent(model);
    AnimationPlayer* animationPlayer = new AnimationPlayer(this, model);
    AddComponent(animationPlayer);
    std::vector<AnimationClip*> prefabClips =
        AssetManager::Instance().GetClips(path);
    for (int i = 0; i < prefabClips.size(); ++i) {
        AnimationClip* clip = new AnimationClip(prefabClips[i]);
        animationPlayer->AddClip(clip);
    }
}

void EnemyAttack::update(float deltaTime) {
    switch (attackType) {
        case (int) AttackState::SWIPE:
            // GetComponent<AnimationPlayer>()->play("swipe-animation");
            break;
        case (int) AttackState::LASER:
            GetComponent<AnimationPlayer>()->play("laser-beamAction");
            break;
        case (int) AttackState::MARK:
            GetComponent<AnimationPlayer>()->play("Key.001Action.002");
            break;
        case (int) AttackState::STOMP:
            GetComponent<AnimationPlayer>()->play("waveAttack");
            break;
    }
}