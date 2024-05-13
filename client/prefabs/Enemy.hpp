#pragma once
#include "Entity.hpp"

class Enemy : public Entity {
    void update() override;
    int32_t TypeID() const { return NetworkObjectTypeID::ENEMY; }
};
