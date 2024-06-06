#include "SwipeAttack.hpp"
#include "CollisionManager.hpp"
#include <iostream>

// use degrees instead of radian
# define PI           180.0
// brackets in the macro are really really important. 
// Otherwise algebra order mistake. since it'll substitute the whole thing into the expression
# define EFFECTRANGE  (2*PI/3)
# define DAMAGERANGE  (PI/3)
# define RADIUS       500
# define HEIGHT       5
# define ANGSPEED     (PI/3)
# define LIFE         (EFFECTRANGE-DAMAGERANGE)/ANGSPEED
# define DAMAGE       15


void SwipeAttack::addCollider(Enemy* owner){
    Collider* attackC = new Collider(this, owner->GetComponent<Collider>());

    std::cout << "boss rotation: " << glm::to_string(attackC->GetRotation()) << std::endl;
    attackC->makeSector(EFFECTRANGE);
    std::cout << "effect range: " << attackC->GetStartAngle() << " to " << attackC->GetEndAngle() << std::endl;
    // swipe starting from right to left because moveBossSwipe 
    attackC->SetStartAngle(attackC->GetStartAngle());
    attackC->SetEndAngle(attackC->GetStartAngle() + DAMAGERANGE);
    std::cout << "damage range: " << attackC->GetStartAngle() << " to " << attackC->GetEndAngle() << std::endl;
    attackC->SetHeight(HEIGHT);
    attackC->SetRadius(RADIUS);
    this->AddComponent(attackC);
}

// Note: enemy should be facing target player
SwipeAttack::SwipeAttack(Enemy* owner) : EnemyAttack(owner){
    this->SetDamage(DAMAGE);
    lifetime = LIFE;
    addCollider(owner);
};

// Note: enemy should be facing target player
SwipeAttack::SwipeAttack(Enemy* owner, int networkId) : EnemyAttack(owner, networkId){
    this->SetDamage(DAMAGE);
    lifetime = LIFE;
    addCollider(owner);
};


void SwipeAttack::update(float deltaTime) {
    lifetime -= deltaTime;
    if (lifetime <= 0) {
        exist = false;
        return;
    }
    Collider* EnemyAttackCollider = this->GetComponent<Collider>();
    std::vector<GameObject*> playersHit = CollisionManager::instance().moveBossSwipe(EnemyAttackCollider, deltaTime * ANGSPEED);
    float newCenterAngle = (EnemyAttackCollider->GetStartAngle() + EnemyAttackCollider->GetEndAngle())/2;
    this->GetComponent<NetTransform>()->SetRotation(glm::vec3(0, newCenterAngle, 0));
    // std::cout << "damage range: " << EnemyAttackCollider->GetStartAngle() << " to " << EnemyAttackCollider->GetEndAngle() << std::endl;
    // std::cout << "attack rotation: " << glm::to_string(this->GetComponent<NetTransform>()->GetRotation()) << std::endl;

    DealDamage(playersHit);
}