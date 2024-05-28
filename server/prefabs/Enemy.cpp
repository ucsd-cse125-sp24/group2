#include "prefabs/Enemy.hpp"
#include "NetTransform.hpp"
#include "CollisionManager.hpp"
#include "Player.hpp"

// Types of Attacks
#include "prefabs/enemySkills/LaserAttack.hpp"
#include "prefabs/enemySkills/MarkedAttack.hpp"
#include "prefabs/enemySkills/StompAttack.hpp"
#include "prefabs/enemySkills/SwipeAttack.hpp"
#include "prefabs/EnemyAttack.hpp"

#include "Health.hpp"
#include "AttackManager.hpp"
#include <iostream>

Enemy::Enemy() : Entity() {
    this->GetComponent<NetTransform>()->SetPosition(glm::vec3(0, 0, 0));
    this->GetComponent<NetTransform>()->SetRotation(glm::vec3(1, 0, 0));
    Collider* hitbox = new Collider(this, this->GetComponent<NetTransform>());
    //TODO: test size values
    hitbox->SetRadius(50);
    hitbox->SetHeight(20);

    AddComponent(hitbox);
    CollisionManager::instance().add(this);
    this->currentPhase = PHASE1;
    Health* h = new Health(this, 100);
    this->AddComponent(h);
}

Enemy::Enemy(int networkId) : Entity(networkId){
    this->GetComponent<NetTransform>()->SetPosition(glm::vec3(0, 0, 0));
    this->GetComponent<NetTransform>()->SetRotation(glm::vec3(1, 0, 0));
    Collider* hitbox = new Collider(this, this->GetComponent<NetTransform>());
    //TODO: test size values
    hitbox->SetRadius(50);
    hitbox->SetHeight(20);

    AddComponent(hitbox); // TODO: decrement player health if they hit the boss
    CollisionManager::instance().add(this);
    this->currentPhase = PHASE1;
    Health* h = new Health(this, 100);
    this->AddComponent(h);
}

void Enemy::update(float deltaTime) {
    s += deltaTime;

    // moves in a circle
    // TODO: use colliderManager.move instead
    GetComponent<NetTransform>()->position += glm::vec3(glm::sin(s), 0, 0);

    // every 5 seconds, attack
    if(std::fmod(s, 5.0) <= deltaTime){
        attack();
        s = std::fmod(s, 5.0);
    }
}

/**
 * attack with type based on enemy phase:
 * swipe (based on proximity)
 * expanding shockwaves
 * throw boulders/AoE targeted attacks
 * rotating lasers
*/
void Enemy::attack(){
    // TODO: check for proximity
    /* if (close to player)
            SwipeAttack(player.position)
    */

    AttackManager::instance().newSwipeAttack();
    std::cout << "SwipeAttack!" << std::endl;

    switch(this->currentPhase){
        case PHASE1: // Default? Do nothing for now
            this->currentPhase = PHASE4;
            break;
        
        case PHASE2: // Stomp / shockwave
            // TODO: AttackManager::instance().newStompAttack();
            break;

        case PHASE3: // Mark / projectile
            // TODO: AttackManager::instance().newMarkedAttack();
            break;

        case PHASE4: // Laser beams
            AttackManager::instance().newLaserAttack();
            std::cout << "LaserAttack!" << std::endl;

            this->currentPhase = PHASE1;
            break;

        default: // For now, nothing. Possibly a phase 5?
            break;
    }
}