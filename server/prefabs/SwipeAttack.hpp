#ifndef SWIPEATTACK_HPP
#define SWIPEATTACK_HPP

#include "Entity.hpp"
#include "Player.hpp"
#include "EnemyAttack.hpp"
#include "Collider.hpp"
#include <vector>

class SwipeAttack : public EnemyAttack {
private:
    float lifetime;
    
    // used by server
    void addCollider(Enemy* owner);

public:
    SwipeAttack(Enemy* owner);
    SwipeAttack(Enemy* owner, int networkId);

    void update(float deltaTime) override;

    std::string ToString() override { return "EnemyAttack - Swipe"; }
};

#endif