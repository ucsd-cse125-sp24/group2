#include "Revive.hpp"
#include "Status.hpp"

Revive::Revive(Player* p) : PlayerSkill() {
    exist = true;
    range = new Collider(this, p->GetComponent<Collider>());
    range->SetRadius(radius);
    this->GetComponent<PlayerSkillType>()->SetState(SkillType::REVIVE);
}

Revive::Revive(Player* p, int networkId) : PlayerSkill(networkId) {
    exist = true;
    range = new Collider(this, p->GetComponent<Collider>());
    range->SetRadius(radius);
    this->GetComponent<PlayerSkillType>()->SetState(SkillType::REVIVE);
}

void Revive::update(float deltaTime) {
    std::vector<GameObject*> hitObjects = CollisionManager::instance().doPlayerEffect(range);
    for (GameObject* g : hitObjects) {
        if (Player* p = dynamic_cast<Player*>(g)) {
            p->GetComponent<Health>()->revive();
        }
    }
    exist = false;
}