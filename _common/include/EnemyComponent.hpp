#pragma once

#include "INetworkComponent.hpp"

enum class AttackState {
    SWIPE,
    LASER,
    MARK,
    STOMP,
    IDLE
};

class EnemyComponent : public INetworkComponent {
    private:
        float animationDuration = 0.0f;
    protected:
        int atk;
    public:
        EnemyComponent(NetworkObject* owner) : INetworkComponent(owner) {}
        void Update(float deltaTime) override;
        void SetState(AttackState atk_state) { atk = (int) atk_state; }
        virtual void Serialize(Packet* pkt) {
            pkt->write_int(atk);
        }
        virtual void Deserialize(Packet* pkt) {
            pkt->read_int(&atk);
        }

        int32_t TypeID() const override { return ENEMY_COMPONENT; }

        std::string ToString() override { return "EnemyComponent"; }
};