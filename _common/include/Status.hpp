#pragma once


#include "INetworkComponent.hpp"
#include "IStatusEffect.hpp"
#include <iostream>
#include "DisplayStatusEffect.hpp"

enum StatusEffectTypeID : int32_t;
class IStatusEffect;
class DisplayStatusEffect;


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

    void Serialize(Packet* packet) override {
        packet->write_int(typeToStatusEffect.size());
        for (auto kvPair : typeToStatusEffect) {
            packet->write_int(kvPair.first); // write StatusEffectTypeID
            packet->write_float(kvPair.second->currTimer); // write currTimer
            packet->write_int(kvPair.second->stacks); // write stacks
        }
    }
    void Deserialize(Packet* packet) override {
        for (auto kvPair : typeToStatusEffect) {
            kvPair.second->stacks = 0; // MARKED FOR DELETION
        }

        int numStatusEffects;
        packet->read_int(&numStatusEffects);
        for (int i = 0; i < numStatusEffects; ++i) {
            StatusEffectTypeID currTypeID;
            packet->read_int((int*)&currTypeID);
            // Add the new status effects
            if (typeToStatusEffect.find(currTypeID) == typeToStatusEffect.end()) {
                typeToStatusEffect[currTypeID] = new DisplayStatusEffect(this);
            }
            packet->read_float(&(typeToStatusEffect[currTypeID]->currTimer));
            packet->read_int(&(typeToStatusEffect[currTypeID]->stacks)); // CLEANSED
        }

        // Cull the old status effects
        for (auto kvPair : typeToStatusEffect) {
            if (kvPair.second->stacks == 0) {
                typeToStatusEffect.erase(kvPair.first); // DELETE THE MARKED
            }
        }
    }

    int32_t TypeID() const override { return STATUS; }
    std::string ToString() override { return "Status"; }
};