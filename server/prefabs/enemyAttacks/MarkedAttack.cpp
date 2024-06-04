#include "MarkedAttack.hpp"
#include <iostream>
#include "CollisionManager.hpp"

# define LATENCY      3
# define DAMAGE       10


// std::vector<double> right_angles = {0, 0.5 * PI, PI, 1.5 * PI};

MarkedAttack::MarkedAttack(Enemy* owner, std::vector<Player*> playerList) : EnemyAttack(owner) {
    for (Player* p : playerList) {
        Collider* attackC = new Collider(this, p->GetComponent<Collider>());
        colliders.push_back(attackC);
    }
    latency = LATENCY;
    SetDamage(DAMAGE);
}

MarkedAttack::MarkedAttack(Enemy* owner, std::vector<Player*> playerList, int networkId) : EnemyAttack(owner, networkId) {
    for (Player* p : playerList) {
        Collider* attackC = new Collider(this, p->GetComponent<Collider>());
        colliders.push_back(attackC);
    }
    latency = LATENCY;
    SetDamage(DAMAGE);
}

void MarkedAttack::update(float deltaTime) {
    latency -= deltaTime;
    if (latency > 0) {
        return;
    }
    for (Collider* attackC : colliders) {
        std::vector<GameObject*> players_hit = CollisionManager::instance().moveBossMark(attackC);
        DealDamage(players_hit);
        exist = false;
    }
}