#pragma once
#include <vector>
#include "IComponent.hpp"

class PlayerCombat : public IComponent {
private:
    struct Combo {
        int comboIndex;
        std::vector<int> sequence;
    };

    std::vector<Combo> combos;

public:
    void AddCombo(const std::vector<int>& sequence);
    bool CheckCombo(int input);
    void ResetCombo(Combo& combo);
    void ResetAllCombos();

    std::string ToString() { return "PlayerCombat"; }
};