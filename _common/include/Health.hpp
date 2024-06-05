#ifndef HEALTH_HPP
#define HEALTH_HPP


#include "INetworkComponent.hpp"


class Health : public INetworkComponent {

public:
    float hp;
    float maxHp;
    int num_needed_revive;
    bool dead;
    bool just_revived;

    Health(NetworkObject* owner);
    Health(NetworkObject* owner, float newHp);

    virtual void Serialize(Packet* packet) override {
        packet->write_float(hp);
    }

    virtual void Deserialize(Packet* packet) override {
        packet->read_float(&hp);
    }

    virtual int32_t TypeID() const override { return HEALTH; }

    bool GetDead() const { return dead; }

    bool GetJustRevived() {
        if (just_revived) {
            just_revived = false;
            return true;
        }
        return false; 
    }

    float GetHealth() const { return hp; }

    void ChangeHealth(float amount);

    void revive();
};


#endif // HEALTH_HPP