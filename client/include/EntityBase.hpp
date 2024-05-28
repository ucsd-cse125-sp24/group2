#pragma once

#include "Entity.hpp"

class EntityBase : public Entity {
    int32_t TypeID() const override { return -1; }

    // Used by server

    virtual void update(float deltaTime) override {}
};