#include "Player.hpp"
#include "components/PlayerCombat.hpp"
#include <iostream>
#include "CollisionManager.hpp"
#include "Collider.hpp"
#include "Health.hpp"
#include "Invincible.hpp"
#include "CooldownComponent.hpp"
#include "MovementStateMachine.hpp"

Player::Player(glm::vec3 position) : Entity() {
    this->GetComponent<NetTransform>()->SetPosition(position);
    alive = true;
    Mover* mover = new Mover(this);
    AddComponent(mover);
    PlayerCombat* playerCombat = new PlayerCombat();
    AddComponent(playerCombat);
    Collider* collider = new Collider(this, this->GetComponent<NetTransform>());
    collider->SetRadius(1);
    collider->SetHeight(2);
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

    // CooldownComponent* cooldownComponent = new CooldownComponent(this);
    // cooldownComponent->AddCooldown("dodge", 1.0f);
    // AddComponent(cooldownComponent);

    MovementStateMachine* movementStateMachine = new MovementStateMachine(this);
    AddComponent(movementStateMachine);
}

void Player::update(float deltaTime) {
    if (GetComponent<Mover>() != nullptr)
        GetComponent<Mover>()->Update(deltaTime);
    if (GetComponent<Invincible>() != nullptr)
        GetComponent<Invincible>()->update(deltaTime);
    if (GetComponent<CooldownComponent>())
        GetComponent<CooldownComponent>()->Update(deltaTime);
}       

void Player::onDestroy() {
    CollisionManager::instance().remove(this);
}