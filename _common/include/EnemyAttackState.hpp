#pragma once

#include "INetworkComponent.hpp"
#include "AttackState.hpp"

class EnemyAttackState : public INetworkComponent {
    protected:
        int atk;
    public:
        EnemyAttackState(NetworkObject* owner) : INetworkComponent(owner) {}
        void Update(float deltaTime) override;
        void SetState(AttackState atk_state) { atk = (int) atk_state; }
        int GetState() { return atk; }
        virtual void Serialize(Packet* pkt) {
            pkt->write_int(atk);
        }
        virtual void Deserialize(Packet* pkt) {
            pkt->read_int(&atk);
        }
        int32_t TypeID() const override { return ENEMY_ATTACK_STATE; }

        std::string ToString() override { return "EnemyAttackState"; }
}