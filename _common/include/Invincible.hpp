#ifndef INVINCIBLE_HPP
#define INVINCIBLE_HPP

#include "IComponent.hpp"

class Invincible : public IComponent {

public:
    bool isInvincible;
    float duration;

    Invincible(GameObject* owner);
    void makeInvincible(float time);
    void update(float deltaTime);
    virtual std::string ToString();
};

#endif // INVINCIBLE_HPP
