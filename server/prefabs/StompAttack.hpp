#ifndef STOMPATTACK_HPP
#define STOMPATTACK_HPP

#include "Enemy.hpp"
#include "EnemyAttack.hpp"

class StompAttack : public EnemyAttack {
public:
    StompAttack(Enemy* owner) : EnemyAttack(owner){};
    StompAttack(Enemy* owner, int networkId) : EnemyAttack(owner, networkId){};

    void update(float deltaTime) override;

    std::string ToString() override { return "EnemyAttack - Stomp"; }
};

#endif // STOMPATTACK_HPP