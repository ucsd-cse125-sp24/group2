#ifndef SWIPEATTACK_HPP
#define SWIPEATTACK_HPP

#include "Entity.hpp"
#include "Player.hpp"
#include "EnemyAttack.hpp"
#include "Collider.hpp"
#include <vector>

# define PI           3.14159265358979323846

//std::vector<double> right_angles = {0, 0.5 * PI, PI, 1.5 * PI};

class SwipeAttack : public EnemyAttack {
private:
    float lifetime;
    
    // used by server
    void addCollider(Enemy* owner);

public:
    SwipeAttack(Enemy* owner);
    SwipeAttack(Enemy* owner, int networkId);

    void update(float deltaTime) override;

    std::string ToString() override { return "EnemyAttack - Swipe"; }
};

#endif