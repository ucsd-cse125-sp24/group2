#include "SwipeAttack.hpp"
#include "CollisionManager.hpp"

SwipeAttack::SwipeAttack(Player* target) {
    Collider* attackC = new Collider(this, target->GetComponent<Collider>());

    // TODO: test and refactor magic numbers
    attackC->makeSector(0.01);
    attackC->SetStartAngle(attackC->GetStartAngle() + right_angles[0]);
    attackC->SetEndAngle(attackC->GetEndAngle() + right_angles[1]);
    attackC->SetHeight(3);
    attackC->SetRadius(500);
    this->AddComponent(attackC);
}

// TODO : make it sweep
void SwipeAttack::update(float deltaTime) {
    Collider* EnemyAttackCollider = this->GetComponent<Collider>();
    std::vector<GameObject*> playersHit = CollisionManager::instance().moveBossSwipe(this, 0.0); // get players hit at this instant
    
    // instantaneous
    DealDamage(playersHit);
}