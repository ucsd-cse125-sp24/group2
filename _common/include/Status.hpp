#pragma once

#include "INetworkComponent.hpp"
#include "IStatusEffect.hpp"
#include "Health.hpp"
#include "Invincible.hpp"
#include <iostream>

class IStatusEffect;
enum class StatusEffectTypeID : int32_t;

class Status : public INetworkComponent {
    struct EnumClassHash {
        template <typename T> std::size_t operator()(T t) const {
            return static_cast<std::size_t>(t);
        }
    };

protected:
    std::unordered_map<StatusEffectTypeID, IStatusEffect*, EnumClassHash>
        typeToStatusEffect;

public:
    Health* health;        // should turn this into reference
    Invincible* isInvincible;   // should turn this into reference
    float& baseSpeed;    // should turn this into reference
    float armor;         // should turn this into reference

    Status(NetworkObject* owner);

    void AddStatusEffect(IStatusEffect*);
    void RemoveStatusEffect(IStatusEffect*);
    void Update(float deltaTime);

    void Serialize(Packet* packet) override;
    void Deserialize(Packet* packet) override;

    int32_t TypeID() const override { return STATUS; }
    std::string ToString() override;
};