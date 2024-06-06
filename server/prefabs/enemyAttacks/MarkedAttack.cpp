#include "MarkedAttack.hpp"
#include <iostream>
#include "CollisionManager.hpp"

MarkedAttack::MarkedAttack(Enemy* owner, std::vector<Player*> playerList) : EnemyAttack(owner) {
    for (Player* p : playerList) {
        Collider* attackC = new Collider(this, p->GetComponent<Collider>());
        colliders.push_back(attackC);
    }
    latency = LATENCY;
    SetDamage(M_DAMAGE);
}

MarkedAttack::MarkedAttack(Enemy* owner, std::vector<Player*> playerList, int networkId) : EnemyAttack(owner, networkId) {
    for (Player* p : playerList) {
        Collider* attackC = new Collider(this, p->GetComponent<Collider>());
        colliders.push_back(attackC);
    }
    latency = LATENCY;
    SetDamage(M_DAMAGE);
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