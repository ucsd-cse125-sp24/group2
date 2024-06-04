#include "SwipeAttack.hpp"
#include "CollisionManager.hpp"

# define PI           180
# define EFFECTRANGE  2*PI/3
# define DAMAGERANGE  PI/3
# define RADIUS       50
# define HEIGHT       5
# define ANGSPEED     PI/3
# define LIFE         (EFFECTRANGE-DAMAGERANGE)/ANGSPEED
# define DAMAGE       15


void SwipeAttack::addCollider(Enemy* owner){
    Collider* attackC = new Collider(this, owner->GetComponent<Collider>());

    attackC->makeSector(DAMAGERANGE);
    // swipe starting from right to left because moveBossSwipe 
    attackC->SetStartAngle(attackC->GetStartAngle() - (EFFECTRANGE - DAMAGERANGE)/2);
    attackC->SetEndAngle(attackC->GetEndAngle() - (EFFECTRANGE - DAMAGERANGE)/2);
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

    DealDamage(playersHit);
}