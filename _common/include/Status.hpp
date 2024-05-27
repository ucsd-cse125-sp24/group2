#pragma once


#include "IComponent.hpp"
#include "IStatusEffect.hpp"


enum StatusEffectTypeID : int32_t;
class IStatusEffect;

class Status : public IComponent {
protected:
    std::unordered_map<StatusEffectTypeID, IStatusEffect*> typeToStatusEffect;

public:
    float health; // should turn this into reference
    bool isInvincible; // should turn this into reference
    float baseMaxHealth; // should turn this into reference
    float& baseSpeed; // should turn this into reference
    float armor; // should turn this into reference


    Status(GameObject* owner);

    void AddStatusEffect(IStatusEffect*);
    void RemoveStatusEffect(IStatusEffect*);
    void Update(float deltaTime);

    std::string ToString() override { return "Status"; }
};