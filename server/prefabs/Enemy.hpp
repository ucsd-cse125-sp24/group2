#pragma once
#include "Entity.hpp"

class Enemy : public Entity {
public:
    Enemy() : Entity() {}
    Enemy(int networkId) : Entity(networkId) {}
    void update(float deltaTime) override;
    std::string ToString() override { return "Enemy"; }
    int32_t TypeID() const override { return NetworkObjectTypeID::ENEMY; }
};