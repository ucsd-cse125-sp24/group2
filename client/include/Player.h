#ifndef PLAYER_H
#define PLAYER_H


#include "Entity.h"

class Player : public Entity {
    public:
        Player();

        void serialize(Packet*) override;
        void deserialize(Packet*) override;
};


#endif // PLAYER_H
