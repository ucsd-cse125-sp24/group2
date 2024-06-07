#include "LaserAttack.hpp"
#include <iostream>
#include "CollisionManager.hpp"

LaserAttack::LaserAttack(Enemy* owner, int index) : EnemyAttack(owner) {
    this->GetComponent<NetTransform>()->SetPosition(owner->GetComponent<NetTransform>()->GetPosition());
    this->GetComponent<NetTransform>()->SetRotation(owner->GetComponent<NetTransform>()->GetRotation());
    initCollider(owner, index);
    lifetime = L_LIFE;
    SetDamage(L_DAMAGE);
}

LaserAttack::LaserAttack(Enemy* owner, int networkId, int index) : EnemyAttack(owner, networkId) {
    this->GetComponent<NetTransform>()->SetPosition(owner->GetComponent<NetTransform>()->GetPosition());
    this->GetComponent<NetTransform>()->SetRotation(owner->GetComponent<NetTransform>()->GetRotation());
    initCollider(owner, index);
    lifetime = L_LIFE;
    SetDamage(L_DAMAGE);
}

void LaserAttack::initCollider(Enemy* owner, int index) {
    attackC = new Collider(this, owner->GetComponent<Collider>());
    attackC->makeSector(ANGRANGE);
    attackC->SetStartAngle(attackC->GetStartAngle() + index * UNIT/2);
    attackC->SetEndAngle(attackC->GetEndAngle() + index * UNIT/2);
    attackC->SetHeight(L_HEIGHT);
    attackC->SetRadius(L_RADIUS);
}

void LaserAttack::update(float deltaTime) {
    if (lifetime <= 0) {
        exist = false;
        return;
    }
    lifetime -= deltaTime;
    
    std::vector<GameObject*> players_hit;
    players_hit = CollisionManager::instance().moveBossSwipe(attackC, deltaTime * L_ANGSPEED);
    DealDamage(players_hit);

    float newCenterAngle = (attackC->GetStartAngle() + attackC->GetEndAngle())/2;
    glm::vec3 newRotation = glm::vec3(0, newCenterAngle, 0);
    this->GetComponent<NetTransform>()->SetRotation(newRotation);
    printf("it updates");
}