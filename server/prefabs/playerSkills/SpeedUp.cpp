#include "SpeedUp.hpp"
#include "Status.hpp"
#include "SpeedBoost.hpp"

SpeedUp::SpeedUp(Player* p) : PlayerSkill() {
    range = new Collider(this, p->GetComponent<Collider>());
    range->SetRadius(radius);
    this->GetComponent<PlayerSkillType>()->SetState(SkillType::SPEED_BOOST);
}

SpeedUp::SpeedUp(Player* p, int networkId) : PlayerSkill(networkId) {
    range = new Collider(this, p->GetComponent<Collider>());
    range->SetRadius(radius);
    this->GetComponent<PlayerSkillType>()->SetState(SkillType::SPEED_BOOST);
}

void SpeedUp::update(float deltaTime) {
    std::vector<GameObject*> hitObjects = CollisionManager::instance().doPlayerEffect(range);
    for (GameObject* g : hitObjects) {
        if (Player* p = dynamic_cast<Player*>(g)) {
            p->GetComponent<Status>()->AddStatusEffect(new SpeedBoost(p->GetComponent<Status>()));
        }
    }
    exist = false;
}