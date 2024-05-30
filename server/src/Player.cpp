#include "Player.hpp"
#include "components/PlayerCombat.hpp"
#include <iostream>
#include "CollisionManager.hpp"
#include "Collider.hpp"
#include "Health.hpp"
#include "Invincible.hpp"

Player::Player() : Entity() {
    alive = true;
    Mover* mover = new Mover(this);
    AddComponent(mover);
    PlayerCombat* playerCombat = new PlayerCombat();
    AddComponent(playerCombat);
    Collider* collider = new Collider(this);
    collider->SetRadius(30);
    collider->SetHeight(10);
    AddComponent(collider);
    CollisionManager::instance().add(this);
    Health* h = new Health(this, 100);
    AddComponent(h);
    Invincible* invinc = new Invincible(this);
    AddComponent(invinc);

    // I = 73, J = 74, K = 75, L = 76
    // TODO make player-specific combos
    playerCombat->AddCombo({74, 74, 74, 74});
    playerCombat->AddCombo({74, 75, 76, 73});
}

void Player::update(float deltaTime) {
    if (GetComponent<Mover>() != nullptr)
        GetComponent<Mover>()->Update();
    if (GetComponent<Invincible>() != nullptr)
        GetComponent<Invincible>()->update(deltaTime);
}

void Player::onDestroy() {
    CollisionManager::instance().remove(this);
}