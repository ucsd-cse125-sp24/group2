#pragma once


#include "INetworkComponent.hpp"
#include "IStatusEffect.hpp"
#include <iostream>

enum StatusEffectTypeID : int32_t;
class IStatusEffect;


class Status : public INetworkComponent {
protected:
    std::unordered_map<StatusEffectTypeID, IStatusEffect*> typeToStatusEffect;

public:
    float health; // should turn this into reference
    bool isInvincible; // should turn this into reference
    float baseMaxHealth; // should turn this into reference
    float& baseSpeed; // should turn this into reference
    float armor; // should turn this into reference

    Status(NetworkObject* owner);

    void AddStatusEffect(IStatusEffect*);
    void RemoveStatusEffect(IStatusEffect*);
    void Update(float deltaTime);

    void Serialize(Packet* packet) override;
    void Deserialize(Packet* packet) override;

    int32_t TypeID() const override { return STATUS; }
    std::string ToString() override;
};