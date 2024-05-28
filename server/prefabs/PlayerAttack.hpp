#ifndef PLAYERATTACK_HPP
#define PLAYERATTACK_HPP

#include "Entity.hpp"
#include "Player.hpp"
#include "Collider.hpp"

class PlayerAttack : public Entity {
private:
    int damage = 10;
    bool exist;
    GameObject* target = nullptr;
    float speed = 50.0f;
    Player* playerOwner;

public:
    PlayerAttack();
    PlayerAttack(int networkId);
    void init(Player* player);
    void update(float deltaTime) override;
    void DealDamage();

    std::string ToString() override { return "PlayerAttack"; }
    int32_t TypeID() const override { return NetworkObjectTypeID::PLAYER_ATTACK; }
    
    void SetDamage(int newDamage) { damage = newDamage; }
    void SetSpeed(float newSpeed) { speed = newSpeed; }
    void setTarget(GameObject* newTarget) { target = newTarget; }

    bool isExist() { return exist; }
};



#endif // PLAYERATTACK_HPP