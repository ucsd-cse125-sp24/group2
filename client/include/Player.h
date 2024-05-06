#ifndef PLAYER_H
#define PLAYER_H


#include "Entity.h"
#include "Mover.h"
#include "AnimationPlayer.h"
#include "AnimationClip.h"
#include "Model.h"

class Player : public Entity {
    public:
        Player();

        void serialize(Packet*) override;
        void deserialize(Packet*) override;
};


#endif // PLAYER_H
