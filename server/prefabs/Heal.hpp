#ifndef HEAL_HPP
#define HEAL_HPP

#include "PlayerSkill.hpp"
#include "Collider.hpp"
#include "CollisionManager.hpp"
#include <vector>

class Heal : public PlayerSkill {

public:
    int amount;
    Collider* range;
    float radius = 10.0f;

    Heal(Player* p);
    Heal(Player* p, int networkId);

    void update(float deltaTime) override;
};

#endif // HEAL_HPP