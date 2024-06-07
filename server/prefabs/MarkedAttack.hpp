#ifndef MARKEDATTACK_HPP
#define MARKEDATTACK_HPP

#include "EnemyAttack.hpp"
#include "Player.hpp"
#include "SkillTraits.hpp"

class MarkedAttack : public EnemyAttack {
public:
    Collider* collider;
    float latency;
    float lifetime;
    bool hasExploded = false;

    MarkedAttack(Enemy* owner, Player*);
    MarkedAttack(Enemy* owner, Player* playerList, int networkId);

    void update(float deltaTime) override;
    std::string ToString() override { return "EnemyAttack - Marked"; }
};

#endif // MARKEDATTACK_HPP
