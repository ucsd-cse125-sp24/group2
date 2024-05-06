#include "Player.h"

#include <iostream>

Player::Player() : Entity() {
    Mover* mover = new Mover("../assets/male_basic_walk_30_frames_loop/scene.gltf");
    AddComponent(mover);
    Model* model = new Model();
    AddComponent(model);
    AnimationClip* clip = new AnimationClip();
    AddComponent(clip);
    AnimationPlayer* animPlayer = new AnimationPlayer();
    AddComponent(animPlayer);
    std::cout << "Player::Player()" << std::endl;
}

// TODO: implement serialize and deserialize? unless it gets implemented in entity
void Player::serialize(Packet*) {
    std::cout << "Player::serialize" << std::endl;
}

void Player::deserialize(Packet*) {
    std::cout << "Player::deserialize" << std::endl;
}