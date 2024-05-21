#ifndef HEALTH_HPP
#define HEALTH_HPP


#include "IComponent.hpp"


class Health : public IComponent {
private:
    int hp;

public:
    Health() : IComponent(), hp(0) {}
    Health(GameObject* owner) : IComponent(owner), hp(0) {}
    Health(GameObject* owner, int newHp) : IComponent(owner), hp(newHp) {}

    virtual std::string ToString() {
        return "Health: hp(" + std::to_string(hp) + ")";
    }

    int GetHealth() const { return hp; }

    void SetHealth(int newHp) { hp = newHp; }

    void ChangeHealth(int amount) { hp += amount; }
};


#endif // HEALTH_HPP