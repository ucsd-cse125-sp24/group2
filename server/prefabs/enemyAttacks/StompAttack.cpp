#include "StompAttack.hpp"
#include "CollisionManager.hpp"

// NOTE: StompAttack does NOT have component colliders because it has two
//       (i.e. in order to differentiate inner/outer, use fields not components)
void StompAttack::addColliders() {
    Collider* outerRing = new Collider(this);
    Collider* innerRing = new Collider(this);

    outerRing->SetHeight(ST_HEIGHT);
    innerRing->SetHeight(ST_HEIGHT);

    outerRing->SetRadius(START_RADIUS);
    float innerRad = START_RADIUS - WAVE_WIDTH;
    innerRing->SetRadius(innerRad < 0 ? 0 : innerRad);

    outerC = outerRing;
    innerC = innerRing;
}

StompAttack::StompAttack(Enemy* owner) : EnemyAttack(owner) {
    addColliders();
    lifetime = ST_LIFE;
    latency = ST_LATENCY;
    SetDamage(ST_DAMAGE);
};

StompAttack::StompAttack(Enemy* owner, int networkId)
    : EnemyAttack(owner, networkId) {
    addColliders();
    lifetime = ST_LIFE;
    latency = ST_LATENCY;
    SetDamage(ST_DAMAGE);
};

void StompAttack::update(float deltaTime) {
    if (lifetime <= 0) {
        exist = false;
        return;
    }
    latency -= deltaTime;
    lifetime -= deltaTime;
    if (latency <= 0) {
        if (DEBUG_ST)
            printf("> lifetime is %f\n", lifetime);

        Collider* EnemyAttackCollider = this->GetComponent<Collider>();
        std::vector<GameObject*> playersHit =
            CollisionManager::instance().moveBossStomp(
                outerC, innerC,
                ST_SPEED * deltaTime); // get players hit at this instant

        if (DEBUG_ST)
            printf("> %d players hit\n\n", playersHit.size());

        DealDamage(playersHit);
    }
}