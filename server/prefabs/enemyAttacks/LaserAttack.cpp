#include "LaserAttack.hpp"
#include <iostream>

// PI is in degree, not radian, as rotation is in degree
# define PI           180
# define ANGRANGE     1
# define RADIUS       500
# define HEIGHT       3
# define ANGSPEED     PI/16
# define LIFE         4
# define DAMAGE       10


// std::vector<double> right_angles = {0, 0.5 * PI, PI, 1.5 * PI};

LaserAttack::LaserAttack(Enemy* owner) : EnemyAttack(owner) {
    this->GetComponent<NetTransform>()->SetPosition(owner->GetComponent<NetTransform>()->GetPosition());
    this->GetComponent<NetTransform>()->SetRotation(owner->GetComponent<NetTransform>()->GetRotation());
    initCollider(owner);
    lifetime = LIFE;
    SetDamage(DAMAGE);
}

LaserAttack::LaserAttack(Enemy* owner, int networkId) : EnemyAttack(owner, networkId) {
    this->GetComponent<NetTransform>()->SetPosition(owner->GetComponent<NetTransform>()->GetPosition());
    this->GetComponent<NetTransform>()->SetRotation(owner->GetComponent<NetTransform>()->GetRotation());
    initCollider(owner);
    lifetime = LIFE;
    SetDamage(DAMAGE);
}

void LaserAttack::initCollider(Enemy* owner) {
    attackFront = new Collider(this, owner->GetComponent<Collider>());
    attackFront->makeSector(ANGRANGE);
    // std::cout << "Front: " << attackFront->GetStartAngle() << " " << attackFront->GetEndAngle() << std::endl;
    attackFront->SetHeight(HEIGHT);
    attackFront->SetRadius(RADIUS);

    attackLeft = new Collider(this, attackFront); // initialize with front
    attackLeft->SetStartAngle(attackLeft->GetStartAngle() + PI/2); // turn 90 degree
    attackLeft->SetEndAngle(attackLeft->GetEndAngle() + PI/2);

    attackBack = new Collider(this, attackLeft); // initialize with left
    attackBack->SetStartAngle(attackBack->GetStartAngle() + PI/2); // turn 90 degree
    attackBack->SetEndAngle(attackBack->GetEndAngle() + PI/2);

    attackRight = new Collider(this, attackBack); // initialize with back
    attackRight->SetStartAngle(attackRight->GetStartAngle() + PI/2); // turn 90 degree
    attackRight->SetEndAngle(attackRight->GetEndAngle() + PI/2);
}

void LaserAttack::update(float deltaTime) {
    lifetime -= deltaTime;
    if (lifetime <= 0) {
        exist = false;
        return;
    }
    std::vector<GameObject*> players_hit;
    players_hit = CollisionManager::instance().moveBossSwipe(attackFront, deltaTime * ANGSPEED);
    DealDamage(players_hit);
    // std::cout << "Front: " << attackFront->GetStartAngle() << " " << attackFront->GetEndAngle() << std::endl;
    players_hit = CollisionManager::instance().moveBossSwipe(attackLeft, deltaTime * ANGSPEED);
    DealDamage(players_hit);
    // std::cout << "Left: " << attackLeft->GetStartAngle() << " " << attackLeft->GetEndAngle() << std::endl;
    players_hit = CollisionManager::instance().moveBossSwipe(attackBack, deltaTime * ANGSPEED);
    DealDamage(players_hit);
    // std::cout << "Back: " << attackBack->GetStartAngle() << " " << attackBack->GetEndAngle() << std::endl;
    players_hit = CollisionManager::instance().moveBossSwipe(attackRight, deltaTime * ANGSPEED);
    DealDamage(players_hit);
    // std::cout << "Right: " << attackRight->GetStartAngle() << " " << attackRight->GetEndAngle() << std::endl;

    float newCenterAngle = (attackFront->GetStartAngle() + attackFront->GetEndAngle())/2;
    glm::vec3 newRotation = glm::vec3(0, newCenterAngle, 0);
    this->GetComponent<NetTransform>()->SetRotation(newRotation);
}