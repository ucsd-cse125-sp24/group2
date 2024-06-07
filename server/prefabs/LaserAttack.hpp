#include "EnemyAttack.hpp"
#include "SkillTraits.hpp"

class LaserAttack : public EnemyAttack {
private:
    Collider* attackC;
    float lifetime;

public:
    LaserAttack(Enemy* owner, int index);
    LaserAttack(Enemy* owner, int index, int networkId);
    void update(float deltaTime) override;

    // used by server
    void initCollider(Enemy* owner, int index);
};