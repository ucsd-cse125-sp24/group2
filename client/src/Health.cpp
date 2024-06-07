#include "Health.hpp"


Health::Health(NetworkObject* owner) 
    : INetworkComponent(owner) {}

Health::Health(NetworkObject* owner, float newHp) 
    : INetworkComponent(owner) {}

void Health::ChangeHealth(float amount) {}

void Health::revive() {}