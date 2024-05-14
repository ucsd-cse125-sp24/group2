#include "Entity.hpp"
#include "NetTransform.hpp"

Entity::Entity() : NetworkObject() {
    NetTransform* transform = new NetTransform(this);
    AddComponent(transform);
}

Entity::Entity(int networkId) : NetworkObject(networkId) {
    NetTransform* transform = new NetTransform(this);
    AddComponent(transform);
}