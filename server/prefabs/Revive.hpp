#ifndef REVIVE_HPP
#define REVIVE_HPP

#include "PlayerSkill.hpp"
#include "Collider.hpp"
#include "CollisionManager.hpp"
#include <vector>

class Revive : public PlayerSkill {

public:
    int amount;
    Collider* range;
    float radius = 10.0f;
    float life = 0.3f;
    bool dealt = false;

    Revive(Player* p);
    Revive(Player* p, int networkId);

    void update(float deltaTime) override;
};

#endif // REVIVE_HPP