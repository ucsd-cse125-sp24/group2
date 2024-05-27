#include "prefabs/Enemy.hpp"
#include "NetTransform.hpp"
#include "CollisionManager.hpp"
#include "Player.hpp"
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
    return;
}