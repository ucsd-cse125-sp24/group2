#include "SwipeAttack.hpp"
#include "CollisionManager.hpp"

// TODO : make it sweep
void SwipeAttack::update(float deltaTime) {
    Collider* EnemyAttackCollider = this->GetComponent<Collider>();
    std::vector<GameObject*> playersHit = CollisionManager::instance().moveBossSwipe(EnemyAttackCollider, 0.0); // get players hit at this instant
    
    // instantaneous
    DealDamage(playersHit);
    exist = false;
}