#include "PlayerAttack.hpp"
#include "NetTransform.hpp"
#include "Collider.hpp"
#include "Health.hpp"
#include "CollisionManager.hpp"
#include <iostream>

PlayerAttack::PlayerAttack() : Entity() { exist = true; }

PlayerAttack::PlayerAttack(int networkId) : Entity(networkId) { exist = true; }

void PlayerAttack::init(Player* player) {
    playerOwner = player;
    Collider* attackC = new Collider(this, player->GetComponent<Collider>());
    auto newY = attackC->GetPosition().y + attackC->GetHeight()/2;
    glm::vec3 rotationXZ(attackC->GetRotation().x, 0, attackC->GetRotation().z);
    glm::vec3 normRotationXZ = glm::normalize(rotationXZ);
    auto newX = attackC->GetPosition().x + normRotationXZ.x * attackC->GetRadius();
    auto newZ = attackC->GetPosition().z + normRotationXZ.z * attackC->GetRadius();
    glm::vec3 newPosition(newX, newY, newZ);
    attackC->SetPosition(newPosition);
    attackC->makePoint();
    this->AddComponent(attackC);
    this->GetComponent<NetTransform>()->SetPosition(newPosition);
}

void PlayerAttack::update(float deltaTime) {
    Collider* PlayerAttackCollider = this->GetComponent<Collider>();
    Collider* targetCollider = target->GetComponent<Collider>();
    glm::vec3 targetCenter(targetCollider->GetPosition().x, targetCollider->GetPosition().y + targetCollider->GetHeight()/2, targetCollider->GetPosition().z);
    glm::vec3 direction = glm::normalize(targetCenter - PlayerAttackCollider->GetPosition());
    glm::vec3 newPosition = PlayerAttackCollider->GetPosition() + direction * speed * deltaTime;
    bool hit = CollisionManager::instance().movePlayerAttack(this, target, newPosition);
    if (hit) {
        DealDamage();
    }
}

void PlayerAttack::DealDamage() {
    if (exist) {
        if (target != nullptr) {
            target->GetComponent<Health>()->ChangeHealth(-damage);
        }
        exist = false;
        CollisionManager::instance().remove(this);
    }
}