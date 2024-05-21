#include "PlayerAttack.hpp"
#include "Transform.hpp"
#include "Collider.hpp"
#include "Health.hpp"
#include "CollisionManager.hpp"

PlayerAttack::PlayerAttack() : Entity() { exist = true; }

PlayerAttack::PlayerAttack(int networkId) : Entity(networkId) { exist = true; }

void PlayerAttack::init(Player* player) {
    playerOwner = player;
    Collider* attackC = player->GetComponent<Collider>();
    auto newZ = attackC->GetPosition().z + attackC->GetHeight()/2;
    glm::vec3 rotationXY(attackC->GetRotation().x, attackC->GetRotation().y, 0);
    glm::vec3 normRotationXY = glm::normalize(rotationXY);
    auto newX = attackC->GetPosition().x + normRotationXY.x * attackC->GetRadius();
    auto newY = attackC->GetPosition().y + normRotationXY.y * attackC->GetRadius();
    glm::vec3 newPosision(newX, newY, newZ);
    attackC->SetPosition(newPosision);
    attackC->makePoint();
    this->AddComponent(attackC);
    this->GetComponent<NetTransform>()->position = newPosision;
}

void PlayerAttack::update(float deltaTime) {
    NetTransform* PlayerAttackTransform = this->GetComponent<NetTransform>();
    NetTransform* targetTransform = target->GetComponent<NetTransform>();
    Collider* targetCollider = target->GetComponent<Collider>();
    glm::vec3 targetCenter(targetTransform->GetPosition().x, targetTransform->GetPosition().y, targetTransform->GetPosition().z + targetCollider->GetHeight()/2);
    glm::vec3 direction = glm::normalize(targetCenter - PlayerAttackTransform->GetPosition());
    glm::vec3 newPosition = PlayerAttackTransform->GetPosition() + direction * speed * deltaTime;
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