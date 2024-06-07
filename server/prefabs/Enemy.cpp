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
#include <NetworkManager.hpp>

#define ENEMY_MAX_HEALTH 100

Enemy::Enemy() : Entity() {
    this->GetComponent<NetTransform>()->SetPosition(glm::vec3(0, 0, 0));
    this->GetComponent<NetTransform>()->SetRotation(glm::vec3(0, 0, 0));
    Collider* hitbox = new Collider(this, this->GetComponent<NetTransform>());
    // TODO: test size values
    hitbox->SetRadius(50);
    hitbox->SetHeight(20);

    AddComponent(hitbox);
    CollisionManager::instance().add(this);
    currentPhase = PHASE1;
    prevPhase = PHASE1;
    Health* h = new Health(this, ENEMY_MAX_HEALTH);
    AddComponent(h);
    EnemyComponent* ec = new EnemyComponent(this);
    this->AddComponent(ec);
}

Enemy::Enemy(int networkId) : Entity(networkId) {
    this->GetComponent<NetTransform>()->SetPosition(glm::vec3(0, 0, 0));
    this->GetComponent<NetTransform>()->SetRotation(glm::vec3(0, 0, 0));
    Collider* hitbox = new Collider(this, this->GetComponent<NetTransform>());
    // TODO: test size values
    hitbox->SetRadius(50);
    hitbox->SetHeight(20);

    AddComponent(hitbox); // TODO: decrement player health if they hit the boss?
    CollisionManager::instance().add(this);
    currentPhase = PHASE1;
    prevPhase = PHASE1;
    Health* h = new Health(this, ENEMY_MAX_HEALTH);
    AddComponent(h);
    EnemyComponent* ec = new EnemyComponent(this);
    this->AddComponent(ec);
}

void Enemy::update(float deltaTime) {
    s += deltaTime;

    // TODO: use colliderManager.move
    int health = GetComponent<Health>()->hp;

    if (health >= 3 / (float)4 * ENEMY_MAX_HEALTH) {
        currentPhase = PHASE1;
    } else if (health >= 2 / (float)4 * ENEMY_MAX_HEALTH) {
        currentPhase = PHASE2;

        if (prevPhase == PHASE1) {
            // TODO Send go next phase
            NetworkManager::instance().send_next_phase();
        }
    } else if (health >= 1 / (float)4 * ENEMY_MAX_HEALTH) {
        currentPhase = PHASE3;

        if (prevPhase == PHASE2) {
            NetworkManager::instance().send_next_phase();
        }
    } else if (health > 0) {
        currentPhase = PHASE4;

        if (prevPhase == PHASE3) {
            NetworkManager::instance().send_next_phase();
        }
    } else {
        currentPhase = (EnemyState)99;

        if (prevPhase == PHASE4) {
            NetworkManager::instance().send_next_phase();
        }
    }

    // every 5 seconds, attack
    if (std::fmod(s, 5.0) <= deltaTime) {
        attack();
        s = std::fmod(s, 5.0);
    } else {
        // J: want to play idle animation when we are not attacking
        GetComponent<EnemyComponent>()->SetState(AttackState::IDLE);
    }

    // TODO: enemy phase logic
    prevPhase = currentPhase;
}

/**
 * attack with type based on enemy phase:
 * swipe (based on proximity)
 * expanding shockwaves
 * throw boulders/AoE targeted attacks
 * rotating lasers
 */
void Enemy::attack() {
    // TODO: check for proximity
    /* if (close to player)
            SwipeAttack(player.position)
    */

    switch (this->currentPhase) {
    case PHASE1: // Swipe
        AttackManager::instance().newSwipeAttack();
        std::cout << "SwipeAttack!" << std::endl;
        break;

    case PHASE2: // Stomp / shockwave
        AttackManager::instance().newStompAttack();
        std::cout << "StompAttack!" << std::endl;
        break;

    case PHASE3: // Mark / projectile
        AttackManager::instance().newMarkedAttack();
        std::cout << "MarkedAttack!" << std::endl;
        break;

    case PHASE4: // Laser beams
        AttackManager::instance().newLaserAttack();
        std::cout << "LaserAttack!" << std::endl;
        break;

    default: // For now, nothing. Possibly a phase 5?
        break;
    }
}