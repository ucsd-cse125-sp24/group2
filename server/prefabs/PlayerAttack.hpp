#ifndef PLAYERATTACK_HPP
#define PLAYERATTACK_HPP

#include "PlayerSkill.hpp"

class PlayerAttack : public PlayerSkill {
private:
    int damage = 10;
    bool exist;
    GameObject* target = nullptr;
    float speed = 200.0f;
    Player* playerOwner;

public:
    PlayerAttack();
    PlayerAttack(int networkId);
    void update(float deltaTime) override;

    // used by server
    void init(Player* player);
    void DealDamage();

    void SetDamage(int newDamage) { damage = newDamage; }
    void SetSpeed(float newSpeed) { speed = newSpeed; }
    void setTarget(GameObject* newTarget) { target = newTarget; }

    bool isExist() { return exist; }
};



#endif // PLAYERATTACK_HPP