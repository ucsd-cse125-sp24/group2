#ifndef SWIPEATTACK_HPP
#define SWIPEATTACK_HPP

#include "Entity.hpp"
#include "Player.hpp"
#include "EnemyAttack.hpp"
#include "Collider.hpp"
#include <vector>

# define PI           3.14159265358979323846

std::vector<double> right_angles = {0, 0.5 * PI, PI, 1.5 * PI};

class SwipeAttack : public EnemyAttack {
private:
    Player* target; // TODO: remove eventually if not needed

public:
    SwipeAttack(Enemy* owner, Player* target) : EnemyAttack(owner){
        Collider* attackC = new Collider(this, target->GetComponent<Collider>());

        // TODO: test and refactor magic numbers
        attackC->makeSector(0.01);
        attackC->SetStartAngle(attackC->GetStartAngle() + right_angles[0]);
        attackC->SetEndAngle(attackC->GetEndAngle() + right_angles[1]);
        attackC->SetHeight(3);
        attackC->SetRadius(500);
        this->AddComponent(attackC);
    };

    SwipeAttack(Enemy* owner, Player* target, int networkId) : EnemyAttack(owner, networkId){
        Collider* attackC = new Collider(this, target->GetComponent<Collider>());

        // TODO: test and refactor magic numbers
        attackC->makeSector(0.01);
        attackC->SetStartAngle(attackC->GetStartAngle() + right_angles[0]);
        attackC->SetEndAngle(attackC->GetEndAngle() + right_angles[1]);
        attackC->SetHeight(3);
        attackC->SetRadius(500);
        this->AddComponent(attackC);
    };

    void update(float deltaTime) override;

    std::string ToString() override { return "EnemyAttack - Swipe"; }
};

#endif