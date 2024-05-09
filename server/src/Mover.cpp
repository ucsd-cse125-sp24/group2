#include "Mover.hpp"
#include "NetTransform.hpp"


Mover::Mover(NetworkObject* owner) : INetworkComponent(owner) {
    NetTransform* transform = owner->GetComponent<NetTransform>();
    position = transform->position;
    rotation = transform->rotation;
}

void Mover::UpdatePhysics() {
    position += 
}

void Mover::Serialize(Packet* packet) {
    packet->write_float(speed);
}

void Mover::Deserialize(Packet* packet) {
    
}