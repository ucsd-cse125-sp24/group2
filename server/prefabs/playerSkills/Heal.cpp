#include "Heal.hpp"
#include "Status.hpp"
#include "HealEffect.hpp"

Heal::Heal(Player* p) : PlayerSkill() {
    range = new Collider(this, p->GetComponent<Collider>());
    range->SetRadius(radius);
    this->GetComponent<PlayerSkillType>()->SetState(SkillType::HEAL);
}

Heal::Heal(Player* p, int networkId) : PlayerSkill(networkId) {
    range = new Collider(this, p->GetComponent<Collider>());
    range->SetRadius(radius);
    this->GetComponent<PlayerSkillType>()->SetState(SkillType::HEAL);
}

void Heal::update(float deltaTime) {
    std::vector<GameObject*> hitObjects = CollisionManager::instance().doPlayerEffect(range);
    for (GameObject* g : hitObjects) {
        if (Player* p = dynamic_cast<Player*>(g)) {
            p->GetComponent<Status>()->AddStatusEffect(new HealEffect(p->GetComponent<Status>()));
        }
    }
    exist = false;
}