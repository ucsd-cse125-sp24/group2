#pragma once    


#include "IComponent.hpp"
#include "Cooldown.hpp"


class CooldownComponent : public IComponent {
protected:
    std::unordered_map<std::string, Cooldown*> cooldowns;

public:
    CooldownComponent(GameObject* owner) : IComponent(owner) {}
    ~CooldownComponent();

    void AddCooldown(std::string name, float baseCd);
    void RemoveCooldown(std::string name);

    bool UseAbility(std::string name);
    void Update(float deltaTime);
    bool IsReady(std::string name);

    std::string ToString() { return "CooldownComponent"; }
};