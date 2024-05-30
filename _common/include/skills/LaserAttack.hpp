#include "EnemyAttack.hpp"

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
    void initCollider(Enemy* owner);
    void update(float deltaTime) override;
};