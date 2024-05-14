#include "Player.hpp"
#include <iostream>


Player::Player() : Entity() {
    Mover* mover = new Mover(this);
    AddComponent(mover);
}

void Player::update() {
    if (GetComponent<Mover>() != nullptr)
        GetComponent<Mover>()->Update();
}

// Already implemented in _common/.../Player.hpp
// int32_t Player::TypeID() const { return NetworkObjectTypeID::PLAYER; }