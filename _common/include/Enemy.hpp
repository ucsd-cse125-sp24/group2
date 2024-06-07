#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.hpp"

#define ENEMY_MAX_HEALTH 2000

class Enemy : public Entity {
public:
    enum EnemyState { PHASE1, PHASE2, PHASE3, PHASE4, PHASE5 };

    Enemy();
    Enemy(int networkId);
    Enemy(std::string path, int networkId);
    void update(float deltaTime) override;
    void attack();
    EnemyState GetEnemyState() const { return currentPhase; }
    std::string ToString() override { return "Enemy"; }
    int32_t TypeID() const override { return NetworkObjectTypeID::ENEMY; }
    float s = 0; // time in milliseconds
    float attackDuration;

private:
    EnemyState currentPhase;
    EnemyState prevPhase;
};

#endif