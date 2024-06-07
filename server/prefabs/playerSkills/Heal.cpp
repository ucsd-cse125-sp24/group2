#include "Heal.hpp"
#include "Status.hpp"
#include "HealEffect.hpp"

Heal::Heal(Player* p) : PlayerSkill() {
    range = new Collider(this, p->GetComponent<Collider>());
    range->SetRadius(radius);
    this->GetComponent<PlayerSkillType>()->SetState(SkillType::HEAL);
    this->GetComponent<NetTransform>()->SetPosition(p->GetComponent<NetTransform>()->GetPosition());
}

Heal::Heal(Player* p, int networkId) : PlayerSkill(networkId) {
    range = new Collider(this, p->GetComponent<Collider>());
    range->SetRadius(radius);
    this->GetComponent<PlayerSkillType>()->SetState(SkillType::HEAL);
    this->GetComponent<NetTransform>()->SetPosition(p->GetComponent<NetTransform>()->GetPosition());
}

void Heal::update(float deltaTime) {
    if (!dealt) {
        std::vector<GameObject*> hitObjects = CollisionManager::instance().doPlayerEffect(range);
        for (GameObject* g : hitObjects) {
            if (Player* p = dynamic_cast<Player*>(g)) {
                p->GetComponent<Status>()->AddStatusEffect(new HealEffect(p->GetComponent<Status>()));
            }
        }
        dealt = true;
    }

    life -= deltaTime;
    if (life < 0) {
        exist = false;
    }
}