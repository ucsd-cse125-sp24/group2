#ifndef ENEMYATTACK_HPP
#define ENEMYATTACK_HPP

#include "Entity.hpp"
#include "Enemy.hpp"
#include "Collider.hpp"

class EnemyAttack : public Entity {
private:
    int damage;
    bool exist;
    Collider* attackC;

public:
    EnemyAttack();
    EnemyAttack(int networkId);
    void DealDamage(std::vector<GameObject*> players_hit);

    std::string ToString() override { return "EnemyAttack"; }
    int32_t TypeID() const override { return NetworkObjectTypeID::ENEMY_ATTACK; }
    
    void SetDamage(int newDamage) { damage = newDamage; }

    bool isExist() { return exist; }
};



#endif // ENEMYATTACK_HPP