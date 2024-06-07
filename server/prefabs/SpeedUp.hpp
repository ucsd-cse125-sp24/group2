#ifndef SPEEDUP_HPP
#define SPEEDUP_HPP

#include "PlayerSkill.hpp"
#include "Collider.hpp"
#include "CollisionManager.hpp"
#include <vector>

class SpeedUp : public PlayerSkill {

public:
    int amount;
    Collider* range;
    float radius = 10.0f;
    float life = 0.3f;
    bool dealt = false;

    SpeedUp(Player* p);
    SpeedUp(Player* p, int networkId);

    void update(float deltaTime) override;
};

#endif // SPEEDUP_HPP