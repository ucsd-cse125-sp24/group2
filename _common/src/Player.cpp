#include "Player.hpp"
#include <iostream>

#include "NetTransform.hpp"


Player::Player() : Entity() {
    Mover* mover = new Mover(this);
    AddComponent(mover);
}

void Player::update() {
    GetComponent<Mover>()->Update();
}

int32_t Player::TypeID() const { return NetworkObjectTypeID::PLAYER; }