#include "Player.h"

#include <iostream>

Player::Player() : Entity() {
    std::cout << "Player::Player()" << std::endl;
}

// TODO: implement serialize and deserialize? unless it gets implemented in entity
void Player::serialize(Packet*) {
    std::cout << "Player::serialize" << std::endl;
}

void Player::deserialize(Packet*) {
    std::cout << "Player::deserialize" << std::endl;
}