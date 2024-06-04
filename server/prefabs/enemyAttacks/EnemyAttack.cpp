#include "EnemyAttack.hpp"
#include "NetTransform.hpp"
#include "Health.hpp"
#include "Invincible.hpp"
#include "Player.hpp"

#include <iostream>

EnemyAttack::EnemyAttack(Enemy* owner) : Entity() { 
    enemy = owner;
    exist = true;
}

EnemyAttack::EnemyAttack(Enemy* owner, int networkId) : Entity(networkId) { 
    enemy = owner;
    exist = true;
}

EnemyAttack::EnemyAttack(int attackType, int networkId) {}

void EnemyAttack::DealDamage(std::vector<GameObject*> entity_hit) {
    if (exist) {
        for (GameObject* entity : entity_hit) {
            if (entity != enemy) {
                if (!(entity->GetComponent<Invincible>() != nullptr
                    && entity->GetComponent<Invincible>()->isInvincible)) {
                    
                    entity->GetComponent<Health>()->ChangeHealth(-damage);
                    std::cout << "New player health: " << entity->GetComponent<Health>()->GetHealth() << std::endl;
                    entity->GetComponent<Invincible>()->makeInvincible(1);

                    if(entity->GetComponent<Health>()->GetHealth() <= 0){
                        dynamic_cast<Player*>(entity)->alive = false;
                    }
                }
            }
        }
    }
}