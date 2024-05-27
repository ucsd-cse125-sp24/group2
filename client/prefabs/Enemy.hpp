#pragma once
#include "Entity.hpp"

class Enemy : public Entity {
public:
    Enemy(int networkId) : Entity(networkId) {}
    void update(float deltaTime) override;
    int32_t TypeID() const { return NetworkObjectTypeID::ENEMY; }
};
