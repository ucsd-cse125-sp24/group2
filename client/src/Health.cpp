#include "Health.hpp"


Health::Health(NetworkObject* owner) 
    : INetworkComponent(owner) {}

Health::Health(NetworkObject* owner, int newHp) 
    : INetworkComponent(owner) {}

void Health::SetHealth(int newHp) {}

void Health::ChangeHealth(int amount) {}