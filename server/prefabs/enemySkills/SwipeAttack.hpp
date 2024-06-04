#ifndef SWIPEATTACK_HPP
#define SWIPEATTACK_HPP

#include "Entity.hpp"
#include "Player.hpp"
#include "EnemyAttack.hpp"
#include "Collider.hpp"
#include <vector>
#include "SkillTraits.hpp"

class SwipeAttack : public EnemyAttack {
private:
    float lifetime;
    void addCollider(Enemy* owner);

public:
    SwipeAttack(Enemy* owner);
    SwipeAttack(Enemy* owner, int networkId);

    void update(float deltaTime) override;

    std::string ToString() override { return "EnemyAttack - Swipe"; }
};

#endif