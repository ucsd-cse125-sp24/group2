#include "EnemyAttack.hpp"
#include "SkillTraits.hpp"

class LaserAttack : public EnemyAttack {
private:
    Collider* attackFront;
    Collider* attackBack;
    Collider* attackLeft;
    Collider* attackRight;
    float lifetime;

public:
    LaserAttack(Enemy* owner);
    LaserAttack(Enemy* owner, int networkId);
    void update(float deltaTime) override;

    // used by server
    void initCollider(Enemy* owner);
};