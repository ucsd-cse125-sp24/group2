#ifndef PLAYERSKILL_HPP
#define PLAYERSKILL_HPP

#include "Entity.hpp"
#include "Player.hpp"

class PlayerSkill : public Entity {

public:
    PlayerSkill() : Entity() {}
    PlayerSkill(int networkId) : Entity(networkId) {}

    std::string ToString() override { return "PlayerSkill"; }
    int32_t TypeID() const override { return NetworkObjectTypeID::PLAYER_SKILL; }  
};



#endif // PLAYERSKILL_HPP