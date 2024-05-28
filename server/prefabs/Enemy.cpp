#include "prefabs/Enemy.hpp"
#include "NetTransform.hpp"
#include "CollisionManager.hpp"
#include "Player.hpp"
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
    // GetComponent<NetTransform>()->position += glm::vec3(glm::sin(s), 0, 0);

    // every 5 seconds, attack
    if(std::fmod(s, 5.0) <= deltaTime){
        attack();
        s = std::fmod(s, 5.0);
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
    AttackManager::instance().newLaserAttack();
    std::cout << "LaserAttack!" << std::endl;
}