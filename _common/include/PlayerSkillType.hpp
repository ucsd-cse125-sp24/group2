#pragma once

#include "INetworkComponent.hpp"

enum class SkillType {
    PLAYER_ATTACK,
    HEAL,
    REVIVE,
    SPEED_BOOST
};

class PlayerSkillType : public INetworkComponent {
    protected:
        int type;
    public:
        PlayerSkillType(NetworkObject* owner) : INetworkComponent(owner) {}
        void Update(float deltaTime) override;
        void SetState(SkillType skillType) { type = (int) skillType; }
        int GetState() { return type; }
        virtual void Serialize(Packet* pkt) {
            pkt->write_int(type);
        }
        virtual void Deserialize(Packet* pkt) {
            pkt->read_int(&type);
        }

        int32_t TypeID() const override { return SKILL_TYPE; }

        std::string ToString() override { return "SkillType"; }
};