#include "Player.hpp"
#include <iostream>
#include "components/PlayerCombat.hpp"
#include "../_common/include/Status.hpp"
#include "SpeedBoost.hpp"
#include "CollisionManager.hpp"
#include "Collider.hpp"
#include "Health.hpp"
#include "Invincible.hpp"
#include "CooldownComponent.hpp"
#include "MovementStateMachine.hpp"

Player::Player(glm::vec3 position, int clientId) : Entity() {
    this->clientId = clientId;
    this->GetComponent<NetTransform>()->SetPosition(position);
    alive = true;
    Mover* mover = new Mover(this);
    AddComponent(mover);
    PlayerCombat* playerCombat = new PlayerCombat();
    AddComponent(playerCombat);
    Collider* collider = new Collider(this, this->GetComponent<NetTransform>());
    collider->SetRadius(0.5f);
    collider->SetHeight(2);
    AddComponent(collider);
    CollisionManager::instance().add(this);
    Health* h = new Health(this, 100);
    AddComponent(h);
    Invincible* invinc = new Invincible(this);
    AddComponent(invinc);
    Status* status = new Status(this);
    AddComponent(status);

    // I = 73, J = 74, K = 75, L = 76
    // TODO make player-specific combos
    playerCombat->AddCombo(attack1, 1);
    playerCombat->AddCombo(attack2, 2);
    playerCombat->AddCombo(heal, 3);
    playerCombat->AddCombo(revive, 4);
    playerCombat->AddCombo(speedBoost, 5);

    // CooldownComponent* cooldownComponent = new CooldownComponent(this);
    // cooldownComponent->AddCooldown("dodge", 1.0f);
    // AddComponent(cooldownComponent);

    MovementStateMachine* movementStateMachine = new MovementStateMachine(this);
    AddComponent(movementStateMachine);
}

void Player::update(float deltaTime) {}

void Player::onDestroy() { CollisionManager::instance().remove(this); }
