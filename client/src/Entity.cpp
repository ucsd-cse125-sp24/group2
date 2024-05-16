#include "Entity.hpp"
#include "Transform.hpp"

void Entity::serialize(Packet* packet) {
    packet->write_int(networkId());
    packet->write_vec3(GetComponent<Transform>()->GetPosition());
    packet->write_vec3(GetComponent<Transform>()->GetRotation());
    packet->write_vec3(GetComponent<Transform>()->GetScale());
}

void Entity::deserialize(Packet* packet) {}