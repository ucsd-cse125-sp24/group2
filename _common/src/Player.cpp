#include "Player.hpp"
#include <iostream>

#include "NetTransform.hpp"
#include "Model.h"
#include "AnimationClip.h"
#include "AnimationPlayer.h"


Player::Player() : Entity() {
    Mover* mover = new Mover(this);
    AddComponent(mover);
}

Player::Player(std::string path) : Entity() {
    Mover* mover = new Mover(this);
    AddComponent(mover);
    Model* model = new Model(this, path);
    AddComponent(model);
    AnimationClip* animationClip = new AnimationClip(this, path, model);
    AddComponent(animationClip);
    AnimationPlayer* animationPlayer = new AnimationPlayer(this, animationClip);
    AddComponent(animationPlayer);
}

void Player::update() {
    if (GetComponent<Mover>() != nullptr)
        GetComponent<Mover>()->Update();
}

void Player::UpdateModel() {
    // Should probably have some sort of render/animation component to make appropriate method calls on
    // if (GetComponent<AnimationPlayer>() != nullptr) {
    //     GetComponent<AnimationPlayer>()->???();
    // }
}

int32_t Player::TypeID() const { return NetworkObjectTypeID::PLAYER; }