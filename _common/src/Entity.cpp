#include "Entity.hpp"
#include "NetTransform.hpp"
#include "Transform.hpp"

Entity::Entity() : NetworkObject() {
    NetTransform* nettransform = new NetTransform(this);
    AddComponent(nettransform);
    Transform* transform = new Transform(this);
    AddComponent(transform);
}

Entity::Entity(int networkId) : NetworkObject(networkId) {
    NetTransform* nettransform = new NetTransform(this);
    AddComponent(nettransform);
    Transform* transform = new Transform(this);
    AddComponent(transform);
}