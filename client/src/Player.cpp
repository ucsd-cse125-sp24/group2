#include "Player.h"

#include <iostream>

Player::Player() : Entity() {
    Mover* mover = new Mover();
    mover->SetOwner(this);
    AddComponent(mover);
    Model* model = new Model("../assets/male_basic_walk_30_frames_loop/scene.gltf");
    model->SetOwner(this);
    AddComponent(model);
    AnimationClip* clip = new AnimationClip("../assets/male_basic_walk_30_frames_loop/scene.gltf", model);
    clip->SetOwner(this);
    AddComponent(clip);
    AnimationPlayer* animPlayer = new AnimationPlayer(clip);
    animPlayer->SetOwner(this);
    AddComponent(animPlayer);
}

// TODO: implement serialize and deserialize? unless it gets implemented in entity
void Player::serialize(Packet*) {
    std::cout << "Player::serialize" << std::endl;
}

void Player::deserialize(Packet*) {
    std::cout << "Player::deserialize" << std::endl;
}