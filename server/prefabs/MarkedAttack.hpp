#ifndef MARKEDATTACK_HPP
#define MARKEDATTACK_HPP

#include "EnemyAttack.hpp"
#include "Player.hpp"
#include "SkillTraits.hpp"

class MarkedAttack : public EnemyAttack {
public:
    std::vector<Collider*> colliders;
    float latency;
    float lifetime;
    bool hasExploded = false;

    MarkedAttack(Enemy* owner, std::vector<Player*> playerList);
    MarkedAttack(Enemy* owner, std::vector<Player*> playerList, int networkId);

    void update(float deltaTime) override;
    std::string ToString() override { return "EnemyAttack - Marked"; }
};

#endif // MARKEDATTACK_HPP
