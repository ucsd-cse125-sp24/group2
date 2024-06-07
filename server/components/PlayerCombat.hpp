#pragma once
#include <vector>
#include "IComponent.hpp"

#define COMBO_RESET_TIME 0.5172 * 4

class PlayerCombat : public IComponent {
private:
    float timeSinceLastInput = 0;
    bool shouldResetCombo = true;
    struct Combo {
        int comboIndex;
        std::vector<int> sequence;
    };

    std::vector<Combo> combos;

public:
    void AddCombo(const std::vector<int>& sequence);
    std::vector<int> CheckCombo(int input);
    void ResetCombo(Combo& combo);
    void ResetAllCombos();
    void Update(float deltaTime) override;

    std::string ToString() { return "PlayerCombat"; }
};