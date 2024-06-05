#include "Status.hpp"
#include "Mover.hpp"

void Status::AddStatusEffect(IStatusEffect* statusEffect) {
    StatusEffectTypeID typeIndex = statusEffect->TypeID();
    if (typeToStatusEffect.find(typeIndex) == typeToStatusEffect.end()) { // if not found
        typeToStatusEffect.emplace(typeIndex, statusEffect);
        typeToStatusEffect[typeIndex]->OnAdd();
        typeToStatusEffect[typeIndex]->StartTimer();
    } else if (statusEffect->isStackable) { // if found, and can stack
        typeToStatusEffect[typeIndex]->stacks++;
        std::cout << "Adding stack" << std::endl;
        // typeToStatusEffect[typeIndex]->OnAdd();
    }
}

void Status::RemoveStatusEffect(IStatusEffect* statusEffect) {
    StatusEffectTypeID typeIndex = statusEffect->TypeID();
    if (typeToStatusEffect.find(typeIndex) != typeToStatusEffect.end()) {
        if (typeToStatusEffect[typeIndex]->stacks > 1) {
            typeToStatusEffect[typeIndex]->stacks--;
            typeToStatusEffect[typeIndex]->StartTimer();
        } else {
            typeToStatusEffect.erase(typeIndex);
            statusEffect->OnRemove();
        }
        // statusEffect->OnRemove();
    }
}

void Status::Update(float deltaTime) {
    std::vector<IStatusEffect*> toBeRemoved;
    for (auto kvPair : typeToStatusEffect) {
        IStatusEffect* currStatusEffect = kvPair.second;
        currStatusEffect->Update(deltaTime);
        if (!currStatusEffect->isActive) {
            toBeRemoved.push_back(currStatusEffect);
        }
    }

    for (IStatusEffect* currSE : toBeRemoved) {
        RemoveStatusEffect(currSE);
    }
}

std::string Status::ToString() { return "Status"; }