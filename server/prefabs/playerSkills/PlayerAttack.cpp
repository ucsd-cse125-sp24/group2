#include "PlayerAttack.hpp"
#include "NetTransform.hpp"
#include "Collider.hpp"
#include "Health.hpp"
#include "CollisionManager.hpp"
#include <iostream>

PlayerAttack::PlayerAttack() : PlayerSkill() {}

PlayerAttack::PlayerAttack(int networkId) : PlayerSkill(networkId) {}

void PlayerAttack::init(Player* player) {
    playerOwner = player;
    Collider* attackC = new Collider(this, player->GetComponent<Collider>());
    auto newY = attackC->GetPosition().y + attackC->GetHeight()/2;
    auto newX = attackC->GetPosition().x - attackC->GetRadius() * glm::sin(glm::radians(attackC->GetRotation().y));
    auto newZ = attackC->GetPosition().z + attackC->GetRadius() * glm::cos(glm::radians(attackC->GetRotation().y));
    glm::vec3 newPosition(newX, newY, newZ);
    attackC->SetPosition(newPosition);
    attackC->makePoint();
    this->AddComponent(attackC);
    this->GetComponent<NetTransform>()->SetPosition(newPosition);
    this->GetComponent<NetTransform>()->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
    this->GetComponent<PlayerSkillType>()->SetState(SkillType::PLAYER_ATTACK);
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
            std::cout << "New enemy health: " << target->GetComponent<Health>()->GetHealth() << std::endl;
        }
        exist = false;
    }
}