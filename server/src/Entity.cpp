#include "Entity.hpp"
void Entity::serialize(Packet* packet) {
    packet->write_int(networkId);
    packet->write_vec3(position);
}

void Entity::deserialize(Packet* packet) {}