#include "SwipeAttack.hpp"
#include "CollisionManager.hpp"

void SwipeAttack::addCollider(){
    Collider* attackC = new Collider(this, target->GetComponent<Collider>());

    // TODO: test and refactor magic numbers
    attackC->makeSector(6);
    //attackC->SetStartAngle(attackC->GetStartAngle() + right_angles[0]);
    //attackC->SetEndAngle(attackC->GetEndAngle() + right_angles[1]);
    attackC->SetHeight(3);
    attackC->SetRadius(500);
    this->AddComponent(attackC);
}

SwipeAttack::SwipeAttack(Enemy* owner, Player* target) : EnemyAttack(owner){
    this->target = target;
    addCollider();
};

SwipeAttack::SwipeAttack(Enemy* owner, Player* target, int networkId) : EnemyAttack(owner, networkId){
    this->target = target;
    addCollider();
};

// TODO : make it sweep
void SwipeAttack::update(float deltaTime) {
    Collider* EnemyAttackCollider = this->GetComponent<Collider>();
    std::vector<GameObject*> playersHit = CollisionManager::instance().moveBossSwipe(EnemyAttackCollider, 0.0); // get players hit at this instant
    
    // instantaneous
    DealDamage(playersHit);
    exist = false;
}