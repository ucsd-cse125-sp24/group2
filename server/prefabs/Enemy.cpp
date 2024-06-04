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
#include "EnemyComponent.hpp"
#include "AttackManager.hpp"
#include <iostream>

Enemy::Enemy() : Entity() {
    this->GetComponent<NetTransform>()->SetPosition(glm::vec3(0, 50.0f, 0));
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
    EnemyComponent* ec = new EnemyComponent(this);
    this->AddComponent(ec);
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
    this->currentPhase = PHASE1;
    Health* h = new Health(this, 100);
    this->AddComponent(h);
    EnemyComponent* ec = new EnemyComponent(this);
    this->AddComponent(ec);
}

void Enemy::update(float deltaTime) {
    s += deltaTime;

    // moves in a circle
    // TODO: use colliderManager.move instead
    // GetComponent<NetTransform>()->position += 10.0f * glm::vec3(glm::sin(s), 0, 0);

    // every 5 seconds, attack
    if(std::fmod(s, 5.0) <= deltaTime){
        attack();
        s = std::fmod(s, 5.0);
    } else {
        // J: want to play idle animation when we are not attacking
        GetComponent<EnemyComponent>()->SetState(AttackState::IDLE);
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

    switch(this->currentPhase){
        case PHASE1: // Default? Do nothing for now
            this->currentPhase = PHASE5;
            break;
        
        case PHASE2: // Stomp / shockwave
            // TODO: AttackManager::instance().newStompAttack();
            break;

        case PHASE3: // Mark / projectile
            AttackManager::instance().newMarkedAttack();
            std::cout << "MarkedAttack!" << std::endl;

            this->currentPhase = PHASE1;
            break;

        case PHASE4: // Laser beams
            AttackManager::instance().newLaserAttack();
            std::cout << "LaserAttack!" << std::endl;

            this->currentPhase = PHASE1;
            break;
        
        case PHASE5: // Swipe
            AttackManager::instance().newSwipeAttack();
            std::cout << "SwipeAttack!" << std::endl;

            this->currentPhase = PHASE1;
            break;

        default: // For now, nothing. Possibly a phase 5?
            break;
    }
}