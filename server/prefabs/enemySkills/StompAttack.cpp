#include "StompAttack.hpp"
#include "CollisionManager.hpp"

// NOTE: StompAttack does NOT have component colliders because it has two
//       (i.e. in order to differentiate inner/outer, use fields not components)
void StompAttack::addColliders(){
    Collider* outerRing = new Collider(this);
    Collider* innerRing = new Collider(this);

    outerRing->SetHeight(ST_HEIGHT);
    innerRing->SetHeight(ST_HEIGHT);

    outerRing->SetRadius(START_RADIUS);
    innerRing->SetRadius(START_RADIUS - WAVE_WIDTH);
    
    outerC = outerRing;
    innerC = innerRing;
}

StompAttack::StompAttack(Enemy* owner) : EnemyAttack(owner){
    addColliders();
    lifetime = ST_LIFE;
};

StompAttack::StompAttack(Enemy* owner, int networkId) : EnemyAttack(owner, networkId){
    addColliders();
    lifetime = ST_LIFE;
};

void StompAttack::update(float deltaTime) {
    if(lifetime <= 0){
        exist = false;
        return;
    }
    lifetime -= deltaTime;

    Collider* EnemyAttackCollider = this->GetComponent<Collider>();
    std::vector<GameObject*> playersHit = CollisionManager::instance().moveBossStomp(outerC, innerC, ST_SPEED*deltaTime); // get players hit at this instant
    
    DealDamage(playersHit);
}