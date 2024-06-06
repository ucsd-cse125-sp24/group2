#include "Health.hpp"
#include "Invincible.hpp"

Health::Health(NetworkObject* owner) 
    : INetworkComponent(owner), hp(100.0f), maxHp(100.0f) {
        dead = false;
    }

Health::Health(NetworkObject* owner, float newHp) 
    : INetworkComponent(owner), hp(newHp), maxHp(newHp) {
        dead = false;
    }

void Health::ChangeHealth(float amount) { 
    hp += amount;
    if (!dead && hp <= 0) {
        dead = true;
        num_needed_revive = 4;
    }
    if (hp < 0) {
        hp = 0;
    }
    if (hp > maxHp) {
        hp = maxHp;
    }
}

void Health::revive() {
    if (dead) {
        num_needed_revive--;
        if (num_needed_revive == 0) {
            dead = false;
            just_revived = true;
            hp = maxHp;
            // invincible for 1 seconds immediately after reviving.
            owner->GetComponent<Invincible>()->makeInvincible(1);
        }
    }
}