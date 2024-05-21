#include "Entity.hpp"
#include "NetTransform.hpp"
#include "Transform.hpp"

Entity::Entity() : NetworkObject() {
    NetTransform* nettransform = new NetTransform(this);
    AddComponent(nettransform);
}

Entity::Entity(int networkId) : NetworkObject(networkId) {
    NetTransform* nettransform = new NetTransform(this);
    AddComponent(nettransform);
}