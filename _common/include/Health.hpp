#ifndef HEALTH_HPP
#define HEALTH_HPP


#include "INetworkComponent.hpp"


class Health : public INetworkComponent {
private:
    int hp;

public:
    Health() : INetworkComponent(), hp(0) {}
    Health(NetworkObject* owner) : INetworkComponent(owner), hp(0) {}
    Health(NetworkObject* owner, int newHp) : INetworkComponent(owner), hp(newHp) {}

    virtual void Serialize(Packet* packet) override {
        packet->write_int(hp);
    }

    virtual void Deserialize(Packet* packet) override {
        packet->read_int(&hp);
    }

    virtual int32_t TypeID() const override { return HEALTH; }

    int GetHealth() const { return hp; }

    void SetHealth(int newHp) { hp = newHp; }

    void ChangeHealth(int amount) { hp += amount; }
};


#endif // HEALTH_HPP