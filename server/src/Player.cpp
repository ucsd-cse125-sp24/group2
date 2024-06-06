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

Player::Player(glm::vec3 position) : Entity() {
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
    status->AddStatusEffect(new SpeedBoost(status)); // test
    status->AddStatusEffect(new SpeedBoost(status)); // test
    status->AddStatusEffect(new SpeedBoost(status)); // test

    // I = 73, J = 74, K = 75, L = 76
    // TODO make player-specific combos
    playerCombat->AddCombo(attack1);
    playerCombat->AddCombo(attack2);
    playerCombat->AddCombo(heal);
    playerCombat->AddCombo(revive);
    playerCombat->AddCombo(speedBoost);

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
    if (GetComponent<Status>())
        GetComponent<Status>()->Update(deltaTime);
}       

void Player::onDestroy() {
    CollisionManager::instance().remove(this);
}
