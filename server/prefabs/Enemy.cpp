#include "prefabs/Enemy.hpp"
#include "NetTransform.hpp"
#include "CollisionManager.hpp"
#include "Player.hpp"

// Types of Attacks
#include "prefabs/enemySkills/LaserAttack.hpp"
#include "prefabs/enemySkills/MarkAttack.hpp"
#include "prefabs/enemySkills/StompAttack.hpp"
#include "prefabs/enemySkills/SwipeAttack.hpp"
#include "prefabs/EnemyAttack.hpp"

//#include "Health.hpp"

Enemy::Enemy() : Entity() {
    Collider* hitbox = new Collider(this);
    //TODO: test size values
    hitbox->SetRadius(50);
    hitbox->SetHeight(20);

    AddComponent(hitbox);
    CollisionManager::instance().add(this);
    this->currentPhase = PHASE1;
}

Enemy::Enemy(int networkId) : Entity(networkId){
    Collider* hitbox = new Collider(this);
    //TODO: test size values
    hitbox->SetRadius(50);
    hitbox->SetHeight(20);

    AddComponent(hitbox); // TODO: decrement player health if they hit the boss
    CollisionManager::instance().add(this);
    this->currentPhase = PHASE1;
}

float s = 0; // time in milliseconds
void Enemy::update(float deltaTime) {
    s += deltaTime;

    // moves in a circle
    // TODO: use colliderManager.move instead
    GetComponent<NetTransform>()->position += glm::vec3(glm::sin(s), 0, 0);

    // every 5 seconds, attack
    if(std::fmod(s, 5000.0) == 0.0){
        attack();
    }
}

/**
 * TODO: attack with type based on enemy phase:
 * swipe (based on proximity)
 * expanding shockwaves
 * throw boulders/AoE targeted attacks
 * rotating lasers
*/
void Enemy::attack(){
    // Always check for proximity
    /* if (close to player)
            SwipeAttack(player.position)
    */
    
    EnemyAttack phaseAttack;

    // if(this->currentPhase != PHASE1){
    //     // also attack
    //     EnemyAttack newAttack = new EnemyAttack();
    // }

// TODO: Do this in enemy attack?
    switch(this->currentPhase){
        case PHASE1: // Default? Do nothing for now
            break;
        
        case PHASE2: // Stomp / shockwave
            phaseAttack = new StompAttack();
            break;

        case PHASE3: // Mark / projectile
            phaseAttack = new MarkAttack();
            break;

        case PHASE4: // Laser beams
            phaseAttack = new LaserAttack();
            break;

        default: // For now, nothing. Possibly a phase 5?
            break;
    }
}