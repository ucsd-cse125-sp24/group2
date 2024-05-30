#include "EnemyAttack.hpp"
#include "Player.hpp"

class MarkedAttack : public EnemyAttack {
public:
    std::vector<Collider*> colliders;
    float latency;

    MarkedAttack(Enemy* owner, std::vector<Player*> playerList);
    MarkedAttack(Enemy* owner, std::vector<Player*> playerList, int networkId);
    void update(float deltaTime) override;
};