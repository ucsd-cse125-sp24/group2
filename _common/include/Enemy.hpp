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
    float s = 0; // time in milliseconds

private:
    enum EnemyState { PHASE1, PHASE2, PHASE3, PHASE4, PHASE5 };

    EnemyState currentPhase;
    EnemyState prevPhase;
};

#endif