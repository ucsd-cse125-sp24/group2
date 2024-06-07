#include "Revive.hpp"
#include "Status.hpp"

Revive::Revive(Player* p) : PlayerSkill() {
    range = new Collider(this, p->GetComponent<Collider>());
    range->SetRadius(radius);
    this->GetComponent<PlayerSkillType>()->SetState(SkillType::REVIVE);
    this->GetComponent<NetTransform>()->SetPosition(p->GetComponent<NetTransform>()->GetPosition());
}

Revive::Revive(Player* p, int networkId) : PlayerSkill(networkId) {
    range = new Collider(this, p->GetComponent<Collider>());
    range->SetRadius(radius);
    this->GetComponent<PlayerSkillType>()->SetState(SkillType::REVIVE);
    this->GetComponent<NetTransform>()->SetPosition(p->GetComponent<NetTransform>()->GetPosition());
}

void Revive::update(float deltaTime) {
    if (!dealt) {
        std::vector<GameObject*> hitObjects = CollisionManager::instance().doPlayerEffect(range);
        for (GameObject* g : hitObjects) {
            if (Player* p = dynamic_cast<Player*>(g)) {
                p->GetComponent<Health>()->revive();
            }
        }
        dealt = true;
    }
    
    life -= deltaTime;
    if (life < 0) {
        exist = false;
    }
}