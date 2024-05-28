#ifndef MARKEDATTACK_HPP
#define MARKEDATTACK_HPP

#include "Player.hpp"
#include "Enemy.hpp"
#include "EnemyAttack.hpp"

class MarkedAttack : public EnemyAttack {
private:
    Player* target;

public:
    MarkedAttack(Enemy* owner, Player* player) : EnemyAttack(owner){target = player;};
    MarkedAttack(Enemy* owner, Player* player, int networkId) : EnemyAttack(owner, networkId){target = player;};

    void update(float deltaTime) override;

    std::string ToString() override { return "EnemyAttack - Marked"; }
};

#endif // MARKEDATTACK_HPP