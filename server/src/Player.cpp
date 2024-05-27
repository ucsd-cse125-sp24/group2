#include "Player.hpp"
#include <iostream>
#include "components/PlayerCombat.hpp"
#include "Status.hpp"
#include "SpeedBoost.hpp"

Player::Player() : Entity() {
    Mover* mover = new Mover(this);
    AddComponent(mover);
    PlayerCombat* playerCombat = new PlayerCombat();
    AddComponent(playerCombat);
    Status* status = new Status(this);
    AddComponent(status);
    status->AddStatusEffect(new SpeedBoost(status));

    // I = 73, J = 74, K = 75, L = 76
    // TODO make player-specific combos
    playerCombat->AddCombo({74, 74, 74, 74});
    playerCombat->AddCombo({74, 75, 76, 73});
}

void Player::update(float deltaTime) {
    if (GetComponent<Mover>() != nullptr)
        GetComponent<Mover>()->Update();
    if (GetComponent<Status>())
        GetComponent<Status>()->Update(deltaTime);
}
