#include "Health.hpp"


Health::Health(NetworkObject* owner) 
    : INetworkComponent(owner), hp(0) {}

Health::Health(NetworkObject* owner, int newHp) 
    : INetworkComponent(owner), hp(newHp) {}

void Health::SetHealth(int newHp) { hp = newHp; }

void Health::ChangeHealth(int amount) { hp += amount; }