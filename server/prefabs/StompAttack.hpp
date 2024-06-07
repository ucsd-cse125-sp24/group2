#ifndef STOMPATTACK_HPP
#define STOMPATTACK_HPP

#include "Enemy.hpp"
#include "EnemyAttack.hpp"
#include "Collider.hpp"
#include "SkillTraits.hpp"

class StompAttack : public EnemyAttack {
private:
    float lifetime;
    Collider* outerC;
    Collider* innerC;
    void addColliders();

public:
    StompAttack(Enemy* owner);
    StompAttack(Enemy* owner, int networkId);

    void update(float deltaTime) override;

    std::string ToString() override { return "EnemyAttack - Stomp"; }
};

#endif // STOMPATTACK_HPP