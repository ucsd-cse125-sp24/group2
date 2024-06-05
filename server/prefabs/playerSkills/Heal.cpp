#include "Heal.hpp"

Heal::Heal(Player* p) : PlayerSkill() {
    exist = true;
    range = new Collider(this, p->GetComponent<Collider>());
}

Heal::Heal(Player* p, int networkId) : PlayerSkill(networkId) {
    exist = true;
    range = new Collider(this, p->GetComponent<Collider>());
}

void Heal::update(float deltaTime) {
    std::vector<GameObject*> hitObjects = CollisionManager::instance().movePlayerHeal(range);
    for (GameObject* g : hitObjects) {
        if (Player* p = dynamic_cast<Player*>(g)) {
            // do the healing
        }
    }
    exist = false;
}