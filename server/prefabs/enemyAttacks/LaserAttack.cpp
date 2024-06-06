#include "LaserAttack.hpp"
#include <iostream>
#include "CollisionManager.hpp"

LaserAttack::LaserAttack(Enemy* owner) : EnemyAttack(owner) {
    this->GetComponent<NetTransform>()->SetPosition(owner->GetComponent<NetTransform>()->GetPosition());
    this->GetComponent<NetTransform>()->SetRotation(owner->GetComponent<NetTransform>()->GetRotation());
    initCollider(owner);
    lifetime = L_LIFE;
    SetDamage(L_DAMAGE);
}

LaserAttack::LaserAttack(Enemy* owner, int networkId) : EnemyAttack(owner, networkId) {
    this->GetComponent<NetTransform>()->SetPosition(owner->GetComponent<NetTransform>()->GetPosition());
    this->GetComponent<NetTransform>()->SetRotation(owner->GetComponent<NetTransform>()->GetRotation());
    initCollider(owner);
    lifetime = L_LIFE;
    SetDamage(L_DAMAGE);
}

void LaserAttack::initCollider(Enemy* owner) {
    attackFront = new Collider(this, owner->GetComponent<Collider>());
    attackFront->makeSector(ANGRANGE);
    // std::cout << "Front: " << attackFront->GetStartAngle() << " " << attackFront->GetEndAngle() << std::endl;
    attackFront->SetHeight(L_HEIGHT);
    attackFront->SetRadius(L_RADIUS);

    attackLeft = new Collider(this, attackFront); // initialize with front
    attackLeft->SetStartAngle(attackLeft->GetStartAngle() + UNIT/2); // turn 90 degree
    attackLeft->SetEndAngle(attackLeft->GetEndAngle() + UNIT/2);

    attackBack = new Collider(this, attackLeft); // initialize with left
    attackBack->SetStartAngle(attackBack->GetStartAngle() + UNIT/2); // turn 90 degree
    attackBack->SetEndAngle(attackBack->GetEndAngle() + UNIT/2);

    attackRight = new Collider(this, attackBack); // initialize with back
    attackRight->SetStartAngle(attackRight->GetStartAngle() + UNIT/2); // turn 90 degree
    attackRight->SetEndAngle(attackRight->GetEndAngle() + UNIT/2);
}

void LaserAttack::update(float deltaTime) {
    if (lifetime <= 0) {
        exist = false;
        return;
    }
    lifetime -= deltaTime;
    
    std::vector<GameObject*> players_hit;
    players_hit = CollisionManager::instance().moveBossSwipe(attackFront, deltaTime * L_ANGSPEED);
    DealDamage(players_hit);
    // std::cout << "Front: " << attackFront->GetStartAngle() << " " << attackFront->GetEndAngle() << std::endl;
    players_hit = CollisionManager::instance().moveBossSwipe(attackLeft, deltaTime * L_ANGSPEED);
    DealDamage(players_hit);
    // std::cout << "Left: " << attackLeft->GetStartAngle() << " " << attackLeft->GetEndAngle() << std::endl;
    players_hit = CollisionManager::instance().moveBossSwipe(attackBack, deltaTime * L_ANGSPEED);
    DealDamage(players_hit);
    // std::cout << "Back: " << attackBack->GetStartAngle() << " " << attackBack->GetEndAngle() << std::endl;
    players_hit = CollisionManager::instance().moveBossSwipe(attackRight, deltaTime * L_ANGSPEED);
    DealDamage(players_hit);
    // std::cout << "Right: " << attackRight->GetStartAngle() << " " << attackRight->GetEndAngle() << std::endl;

    float newCenterAngle = (attackFront->GetStartAngle() + attackFront->GetEndAngle())/2;
    glm::vec3 newRotation = glm::vec3(0, newCenterAngle, 0);
    this->GetComponent<NetTransform>()->SetRotation(newRotation);
}