#include "EnemyAttack.hpp"
#include "NetTransform.hpp"
#include "Health.hpp"
#include "Invincible.hpp"
#include "Player.hpp"
#include "CollisionManager.hpp"

#include <iostream>
#include <MovementStateMachine.hpp>

EnemyAttack::EnemyAttack(Enemy* owner) : Entity() {
    enemy = owner;
    exist = true;
}

EnemyAttack::EnemyAttack(Enemy* owner, int networkId) : Entity(networkId) {
    enemy = owner;
    exist = true;
}

EnemyAttack::EnemyAttack(int attackType, int networkId) {}
/**
 * Checks if subject is invincible or dodging attacks
 * TODO: use status effects in future
 */
bool takesDamage(GameObject* subject) {
    MovementStateMachine* msm = subject->GetComponent<MovementStateMachine>();
    bool dodged =
        msm != nullptr && (msm->GetState() == MovementState::DODGE ||
                           msm->GetState() == MovementState::DODGE_START);
    // if(dodged) printf("dodged!\n");
    return (!(subject->isInvincible()) && !dodged);
}

void EnemyAttack::DealDamage(std::vector<GameObject*> entity_hit) {
    if (exist) {
        for (GameObject* entity : entity_hit) {
            if (entity != enemy) {
                if (takesDamage(entity)) {
                    entity->GetComponent<Health>()->ChangeHealth(-damage);
                    std::cout << "New player health: "
                              << entity->GetComponent<Health>()->GetHealth()
                              << std::endl;
                    entity->GetComponent<Invincible>()->makeInvincible(1);

                    if (entity->GetComponent<Health>()->GetHealth() <= 0) {
                        dynamic_cast<Player*>(entity)->alive = false;
                    }
                }
            }
        }
    }
}
void EnemyAttack::update(float deltaTime) {}