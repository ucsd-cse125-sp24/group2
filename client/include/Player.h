#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.hpp"

class Player : public Entity {
    void update(float deltaTime) override;
    int32_t TypeID() const { return NetworkObjectTypeID::PLAYER; }
};

#endif // PLAYER_H
