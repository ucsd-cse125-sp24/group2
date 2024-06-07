#include "MarkedAttack.hpp"
#include <iostream>
#include "CollisionManager.hpp"

MarkedAttack::MarkedAttack(Enemy* owner, Player* player) : EnemyAttack(owner) {
    collider = new Collider(this, player->GetComponent<Collider>());
    latency = LATENCY;
    lifetime = M_LIFETIME;
    SetDamage(M_DAMAGE);
}

MarkedAttack::MarkedAttack(Enemy* owner, Player* player, int networkId)
    : EnemyAttack(owner, networkId) {
    collider = new Collider(this, player->GetComponent<Collider>());
    latency = LATENCY;
    lifetime = M_LIFETIME;
    SetDamage(M_DAMAGE);
}

void MarkedAttack::update(float deltaTime) {
    latency -= deltaTime;
    lifetime -= deltaTime;
    if (latency < 0 && !hasExploded) {
        std::vector<GameObject*> players_hit =
            CollisionManager::instance().moveBossMark(collider);
        DealDamage(players_hit);

        hasExploded = true;
    }

    if (lifetime < 0) {
        exist = false;
    }
}