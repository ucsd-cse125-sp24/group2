#include "Entity.hpp"
#include "NetTransform.hpp"

Entity::Entity() : NetworkObject() {
    NetTransform* transform = new NetTransform(this);
    AddComponent(transform);
}

/**
 * Serializes networkId() and all networkComponents in order that they are stored with networkComponents
*/
void Entity::serialize(Packet* packet) {
    packet->write_int(networkId());
    // packet->write_vec3(position);
    for (INetworkComponent* netComp : networkComponents) {
        netComp->Serialize(packet);
    }
}

void Entity::deserialize(Packet* packet) {
    // TODO: May need to be implemented for client side deserialize
}