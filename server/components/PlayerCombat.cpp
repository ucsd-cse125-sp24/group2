#include "PlayerCombat.hpp"
#include "Health.hpp"

void PlayerCombat::Update(float deltaTime) {
    timeSinceLastInput += deltaTime;

    if (timeSinceLastInput >= COMBO_RESET_TIME && shouldResetCombo) {
        ResetAllCombos();
        shouldResetCombo = false;
    }
}

void PlayerCombat::AddCombo(const std::vector<int>& sequence) {
    Combo combo;
    combo.comboIndex = 0;
    combo.sequence = sequence;
    combos.push_back(combo);
}

std::vector<int> PlayerCombat::CheckCombo(int input) {
    if (owner->GetComponent<Health>()->GetDead()) {
        return {};
    }

    timeSinceLastInput = 0;
    shouldResetCombo = true;

    for (auto& combo : combos) {
        // Check if current input matches combo's input
        if (combo.sequence[combo.comboIndex] != input) {
            // Did not match, reset
            ResetCombo(combo);
            continue;
        }

        combo.comboIndex++;

        // Reached end of combo, success
        if (combo.comboIndex == combo.sequence.size()) {
            ResetAllCombos();
            return combo.sequence;
        }
    }

    return {};
}

void PlayerCombat::ResetCombo(Combo& combo) { combo.comboIndex = 0; }

void PlayerCombat::ResetAllCombos() {
    for (auto& combo : combos) {
        ResetCombo(combo);
    }
}