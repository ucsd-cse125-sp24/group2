#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.hpp"

class Enemy : public Entity {
public:
    Enemy();
    Enemy(int networkId);
    void update(float deltaTime) override;
    void attack();
    std::string ToString() override { return "Enemy"; }
    int32_t TypeID() const override { return NetworkObjectTypeID::ENEMY; }
};

#endif