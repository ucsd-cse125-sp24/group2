#ifndef PLAYERSKILL_HPP
#define PLAYERSKILL_HPP

#include "Entity.hpp"
#include "Player.hpp"
#include "PlayerSkillType.hpp"

class PlayerSkill : public Entity {

public:
    PlayerSkill();
    PlayerSkill(int networkId);

    // used by client
    void initComponent(int skillType);

    void update(float deltaTime) override {}
    std::string ToString() override { return "PlayerSkill"; }
    int32_t TypeID() const override { return NetworkObjectTypeID::PLAYER_SKILL; }  
};



#endif // PLAYERSKILL_HPP