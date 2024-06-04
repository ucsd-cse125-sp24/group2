#include "Enemy.hpp"
#include "NetTransform.hpp"
#include "CollisionManager.hpp"
#include "Player.hpp"

// Types of Attacks
#include "LaserAttack.hpp"
#include "MarkedAttack.hpp"
#include "StompAttack.hpp"
#include "SwipeAttack.hpp"
#include "EnemyAttack.hpp"

#include "Health.hpp"
#include "AttackManager.hpp"
#include <iostream>

Enemy::Enemy() : Entity() {
    this->GetComponent<NetTransform>()->SetPosition(glm::vec3(0, 0, 0));
    this->GetComponent<NetTransform>()->SetRotation(glm::vec3(0, 0, 0));
    Collider* hitbox = new Collider(this, this->GetComponent<NetTransform>());
    //TODO: test size values
    hitbox->SetRadius(50);
    hitbox->SetHeight(20);

    AddComponent(hitbox);
    CollisionManager::instance().add(this);
    this->currentPhase = PHASE1;
    Health* h = new Health(this, 500);
    this->AddComponent(h);
}

Enemy::Enemy(int networkId) : Entity(networkId){
    this->GetComponent<NetTransform>()->SetPosition(glm::vec3(0, 0, 0));
    this->GetComponent<NetTransform>()->SetRotation(glm::vec3(0, 0, 0));
    Collider* hitbox = new Collider(this, this->GetComponent<NetTransform>());
    //TODO: test size values
    hitbox->SetRadius(50);
    hitbox->SetHeight(20);

    AddComponent(hitbox); // TODO: decrement player health if they hit the boss
    CollisionManager::instance().add(this);
    this->currentPhase = PHASE2;
    Health* h = new Health(this, 100);
    this->AddComponent(h);
}

void Enemy::update(float deltaTime) {
    s += deltaTime;

    // TODO: use colliderManager.move

    // every 5 seconds, attack
    if(std::fmod(s, 5.0) <= deltaTime){
        attack();
        s = std::fmod(s, 5.0);
    }

    // TODO: enemy phase logic
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

    switch(this->currentPhase){
        case PHASE1: // Swipe
            AttackManager::instance().newSwipeAttack();
            std::cout << "SwipeAttack!" << std::endl;

            this->currentPhase = PHASE2;
            break;
        
        case PHASE2: // Stomp / shockwave
            AttackManager::instance().newStompAttack();
            std::cout << "StompAttack!" << std::endl;

            //this->currentPhase = PHASE3;
            break;

        case PHASE3: // Mark / projectile
            AttackManager::instance().newMarkedAttack();
            std::cout << "MarkedAttack!" << std::endl;

            this->currentPhase = PHASE4;
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