#include "PlayerAttack.hpp"
#include "Transform.hpp"
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
    this->GetComponent<NetTransform>()->position = newPosition;
    std::cout << "Attack fired! at position: (" << newPosition.x << ", " << newPosition.y << ", " << newPosition.z << ")" << std::endl;
}

void PlayerAttack::update(float deltaTime) {
    NetTransform* PlayerAttackTransform = this->GetComponent<NetTransform>();
    NetTransform* targetTransform = target->GetComponent<NetTransform>();
    Collider* targetCollider = target->GetComponent<Collider>();
    glm::vec3 targetCenter(targetTransform->GetPosition().x, targetTransform->GetPosition().y + targetCollider->GetHeight()/2, targetTransform->GetPosition().z);
    glm::vec3 direction = glm::normalize(targetCenter - PlayerAttackTransform->GetPosition());
    glm::vec3 newPosition = PlayerAttackTransform->GetPosition() + direction * speed * deltaTime;
    bool hit = CollisionManager::instance().movePlayerAttack(this, target, newPosition);
    std::cout << "Attack at position: (" << newPosition.x << ", " << newPosition.y << ", " << newPosition.z << ")" << std::endl;
    if (hit) {
        DealDamage();
    }
}

void PlayerAttack::DealDamage() {
    if (exist) {
        if (target != nullptr) {
            std::cout << "Boss hit, Boss HP change from " << target->GetComponent<Health>()->GetHealth();
            target->GetComponent<Health>()->ChangeHealth(-damage);
            std::cout << " to " << target->GetComponent<Health>()->GetHealth() << std::endl;
        }
        exist = false;
        CollisionManager::instance().remove(this);
    }
}