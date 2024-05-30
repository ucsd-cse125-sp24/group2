#include "SwipeAttack.hpp"
#include "CollisionManager.hpp"

# define DAMAGE       15

void SwipeAttack::init(Player* p){
    this->target = p;
    this->SetDamage(DAMAGE);
}

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
    init(target);
    addCollider();
};

SwipeAttack::SwipeAttack(Enemy* owner, Player* target, int networkId) : EnemyAttack(owner, networkId){
    init(target);
    addCollider();
};

// TODO : make it sweep
void SwipeAttack::update(float deltaTime) {
    Collider* EnemyAttackCollider = this->GetComponent<Collider>();
    std::vector<GameObject*> playersHit = CollisionManager::instance().moveBossSwipe(EnemyAttackCollider, 0.0); // get players hit at this instant
    
    // instantaneous
    DealDamage(playersHit);
    // printf("HIT a swipe attack!\n");
    exist = false;
}