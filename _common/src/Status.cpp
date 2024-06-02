#include "Status.hpp"
#include "Mover.hpp"
#include "DisplayStatusEffect.hpp"


Status::Status(NetworkObject* owner) : 
    INetworkComponent(owner),
    baseSpeed(owner->GetComponent<Mover>()->baseSpeed)
{}

void Status::Serialize(Packet* packet) {
    packet->write_int(typeToStatusEffect.size());
    for (auto kvPair : typeToStatusEffect) {
        packet->write_int(kvPair.first); // write StatusEffectTypeID
        packet->write_float(kvPair.second->currTimer); // write currTimer
        packet->write_int(kvPair.second->stacks); // write stacks
    }
}

void Status::Deserialize(Packet* packet) {
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

// std::string Status::ToString() {
//     std::string output = "Status:";
//     for (auto kvPair : typeToStatusEffect) {
//         output += "\n  Type: " + std::to_string(kvPair.first);
//         output += "\n    Stacks: " + std::to_string(kvPair.second->stacks);
//         output += "\n    currTimer: " + std::to_string(kvPair.second->currTimer);
//     }
//     return output += "\n";
// }