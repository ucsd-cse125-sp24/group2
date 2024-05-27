#include "Status.hpp"
#include "Mover.hpp"


Status::Status(GameObject* owner) : 
    IComponent(owner),
    baseSpeed(owner->GetComponent<Mover>()->baseSpeed) // TODO: Update this to be mover->baseSpeed when branch "dodging" is merged in
{}

void Status::AddStatusEffect(IStatusEffect* statusEffect) {
    StatusEffectTypeID typeIndex = statusEffect->TypeID();
    if (typeToStatusEffect.find(typeIndex) == typeToStatusEffect.end()) { // if not found
        typeToStatusEffect.emplace(typeIndex, statusEffect);
        typeToStatusEffect[typeIndex]->OnAdd();
    } else if (statusEffect->isStackable) { // if found, and can stack
        typeToStatusEffect[typeIndex]->stacks++;
        typeToStatusEffect[typeIndex]->OnAdd();
    }
    typeToStatusEffect[typeIndex]->StartTimer();
}

void Status::RemoveStatusEffect(IStatusEffect* statusEffect) {
    StatusEffectTypeID typeIndex = statusEffect->TypeID();
    if (typeToStatusEffect.find(typeIndex) != typeToStatusEffect.end()) {
        if (typeToStatusEffect[typeIndex]->stacks > 1) {
            typeToStatusEffect[typeIndex]->stacks--;
        } else {
            typeToStatusEffect.erase(typeIndex);
        }
        statusEffect->OnRemove();
    }
}

void Status::Update(float deltaTime) {
    for (auto kvPair : typeToStatusEffect) {
        IStatusEffect* currStatusEffect = kvPair.second;
        currStatusEffect->Update(deltaTime);
        if (!currStatusEffect->isActive) {
            RemoveStatusEffect(currStatusEffect);
        }
    }
}