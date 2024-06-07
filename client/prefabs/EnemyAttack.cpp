#include "EnemyAttack.hpp"
#include "AnimationPlayer.h"
#include "components/RendererComponent.hpp"
#include "AssetManager.hpp"
#include "EnemyComponent.hpp"

#include <iostream>

// note: these are model paths relative to main.cpp
// as that is where AssetManager initializes its modelpath list
std::string swipePath = "../assets/swipe-attack/swipe-attack.gltf";
std::string laserPath = "../assets/laser-attack/laser-beam-model.gltf";
std::string markPath =
    "../assets/projectile-attack/projectile-attack-bone.gltf";
std::string stompPath = "../assets/wave-attack-v3/WaveAttack.gltf";

EnemyAttack::EnemyAttack(Enemy* owner) : Entity() {}

EnemyAttack::EnemyAttack(Enemy* owner, int networkId) : Entity(networkId) {}

EnemyAttack::EnemyAttack(int attackType, int networkId) : Entity(networkId) {
    this->attackType = attackType;
    RendererComponent* meshRenderer;
    Model* model;
    std::string path;
    switch (attackType) {
    case (int)AttackState::SWIPE:
        meshRenderer = new RendererComponent(this, ShaderType::STANDARD);
        GetComponent<NetTransform>()->SetScale(glm::vec3(5.0f));
        model = new Model(AssetManager::Instance().GetModel(swipePath));
        path = swipePath;
        printf("SwipeAttack!\n");
        break;
    case (int)AttackState::LASER:
        // we probably need to tune down stomp attack
        GetComponent<NetTransform>()->SetScale(glm::vec3(30.0f));
        meshRenderer = new RendererComponent(this, ShaderType::ANIMATED);
        model = new Model(AssetManager::Instance().GetModel(laserPath));
        path = laserPath;
        printf("LaserAttack!\n");
        break;
    case (int)AttackState::MARK:
        meshRenderer = new RendererComponent(this, ShaderType::ANIMATED);
        model = new Model(AssetManager::Instance().GetModel(markPath));
        path = markPath;
        printf("MarkedAttack!\n");
        break;
    case (int)AttackState::STOMP:
        GetComponent<NetTransform>()->SetScale(glm::vec3(30.0f));
        meshRenderer = new RendererComponent(this, ShaderType::ANIMATED);
        model = new Model(AssetManager::Instance().GetModel(stompPath));
        path = stompPath;
        printf("StompAttack!\n");
        break;
    default:
        printf("Wrong AttackState!\n");
        return;
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
    glm::vec3 r;
    switch (attackType) {
    case (int)AttackState::SWIPE:
        // GetComponent<AnimationPlayer>()->play("swipe-animation");
        break;
    case (int)AttackState::LASER:
        GetComponent<AnimationPlayer>()->play("laser-spin", false);
        break;
    case (int)AttackState::MARK:
        GetComponent<AnimationPlayer>()->play("projectile-bone-anim", false);
        break;
    case (int)AttackState::STOMP:
        GetComponent<AnimationPlayer>()->play("waveAttack", false);
        break;
    }
}