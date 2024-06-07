#include "SpeedUp.hpp"
#include "Status.hpp"
#include "SpeedBoost.hpp"

SpeedUp::SpeedUp(Player* p) : PlayerSkill() {
    range = new Collider(this, p->GetComponent<Collider>());
    range->SetRadius(radius);
    this->GetComponent<PlayerSkillType>()->SetState(SkillType::SPEED_BOOST);
    this->GetComponent<NetTransform>()->SetPosition(p->GetComponent<NetTransform>()->GetPosition());
}

SpeedUp::SpeedUp(Player* p, int networkId) : PlayerSkill(networkId) {
    range = new Collider(this, p->GetComponent<Collider>());
    range->SetRadius(radius);
    this->GetComponent<PlayerSkillType>()->SetState(SkillType::SPEED_BOOST);
    this->GetComponent<NetTransform>()->SetPosition(p->GetComponent<NetTransform>()->GetPosition());
}

void SpeedUp::update(float deltaTime) {
    if (!dealt) {
        std::vector<GameObject*> hitObjects = CollisionManager::instance().doPlayerEffect(range);
        for (GameObject* g : hitObjects) {
            if (Player* p = dynamic_cast<Player*>(g)) {
                p->GetComponent<Status>()->AddStatusEffect(new SpeedBoost(p->GetComponent<Status>()));
            }
        }
        dealt = true;
    }
    
    life -= deltaTime;
    if (life < 0) {
        exist = false;
    }
}