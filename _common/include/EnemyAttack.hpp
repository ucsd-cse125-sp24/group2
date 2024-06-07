#ifndef ENEMYATTACK_HPP
#define ENEMYATTACK_HPP

#include "Entity.hpp"
#include "Enemy.hpp"
#include "Collider.hpp"
#include <vector>

class EnemyAttack : public Entity {
private:
    int damage;
    Enemy* enemy;
    float stompAttackDelay = 2.0f;

public:
    // used by server
    bool exist;
    EnemyAttack(Enemy* owner);
    EnemyAttack(Enemy* owner, int networkId);
    void DealDamage(std::vector<GameObject*> players_hit);

    // used by client
    int attackType;
    EnemyAttack(int attackType, int networkId);

    std::string ToString() override { return "EnemyAttack"; }
    int32_t TypeID() const override { return NetworkObjectTypeID::ENEMY_ATTACK; }
    void update(float deltaTime) override;
    
    void SetDamage(int newDamage) { damage = newDamage; }

    bool isExist() { return exist; }
};



#endif // ENEMYATTACK_HPP