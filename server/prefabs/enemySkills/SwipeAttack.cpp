#include "SwipeAttack.hpp"
#include "CollisionManager.hpp"
#include <iostream>


void SwipeAttack::addCollider(Enemy* owner){
    Collider* attackC = new Collider(this, owner->GetComponent<Collider>());

    attackC->makeSector(ACTIVE_RANGE);
    // swipe starting from right to left because moveBossSwipe 
    attackC->SetStartAngle(attackC->GetStartAngle() - (FULL_RANGE - ACTIVE_RANGE)/2);
    attackC->SetEndAngle(attackC->GetEndAngle() - (FULL_RANGE - ACTIVE_RANGE)/2);
    attackC->SetHeight(SW_HEIGHT);
    attackC->SetRadius(SW_RADIUS);
    this->AddComponent(attackC);
}

// Note: enemy should be facing target player
SwipeAttack::SwipeAttack(Enemy* owner) : EnemyAttack(owner){
    this->SetDamage(SW_DAMAGE);
    lifetime = SW_LIFE;
    addCollider(owner);
};

// Note: enemy should be facing target player
SwipeAttack::SwipeAttack(Enemy* owner, int networkId) : EnemyAttack(owner, networkId){
    this->SetDamage(SW_DAMAGE);
    lifetime = SW_LIFE;
    addCollider(owner);
};


void SwipeAttack::update(float deltaTime) {
    if (lifetime <= 0) {
        exist = false;
        return;
    }
    lifetime -= deltaTime;
    
    Collider* EnemyAttackCollider = this->GetComponent<Collider>();
    std::vector<GameObject*> playersHit = CollisionManager::instance().moveBossSwipe(EnemyAttackCollider, deltaTime * SW_ANGSPEED);

    DealDamage(playersHit);
}