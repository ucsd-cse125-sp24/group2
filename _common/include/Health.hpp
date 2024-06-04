#ifndef HEALTH_HPP
#define HEALTH_HPP


#include "INetworkComponent.hpp"


class Health : public INetworkComponent {

public:
    int hp;

    Health(NetworkObject* owner);
    Health(NetworkObject* owner, int newHp);

    virtual void Serialize(Packet* packet) override {
        packet->write_int(hp);
    }

    virtual void Deserialize(Packet* packet) override {
        packet->read_int(&hp);
    }

    virtual int32_t TypeID() const override { return HEALTH; }

    int GetHealth() const { return hp; }

    void SetHealth(int newHp);

    void ChangeHealth(int amount);
};


#endif // HEALTH_HPP