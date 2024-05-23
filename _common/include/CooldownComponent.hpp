#pragma once    


#include "IComponent.hpp"


class CooldownComponent : public IComponent {
protected:
    class Cooldown;
    std::unordered_map<std::string, Cooldown*> cooldowns;

    class Cooldown {
    public:
        bool onCooldown = false;
        float baseCooldown;
        float currentTimer = 0.0f;

        Cooldown(float baseCooldown) : baseCooldown(baseCooldown) {}

        void StartTimer() { 
            StartTimer(baseCooldown);
        }
        void StartTimer(float timeStart) {
            onCooldown = true;
            currentTimer = timeStart;
        }
        void UpdateTimer(float deltaTime) { 
            currentTimer = std::max(currentTimer - deltaTime, 0.0f);
            if (currentTimer == 0.0f) onCooldown = false;
        }
        bool IsReady() const { return !onCooldown; }
    };

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