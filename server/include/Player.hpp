#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Entity.hpp"
#include "PlayerCombat.hpp"

class Player : public Entity {
public:
    Player() {
        PlayerCombat* playerCombat = new PlayerCombat();
        AddComponent(playerCombat);

        // I = 73, J = 74, K = 75, L = 76
        // TODO make player-specific combos
        playerCombat->AddCombo({74, 74, 74, 74});
        playerCombat->AddCombo({74, 75, 76, 73});
    }
    glm::vec2 inputs = glm::vec2();
    glm::vec3 velocity = glm::vec3();
    float speed = 0.2f;
    virtual void update() override;
    int32_t TypeID() const override;
};