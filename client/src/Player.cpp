#include "Player.h"

#include "../include/Mover.h"
#include "../include/Model.h"
#include "../include/AnimationClip.h"
#include "../include/AnimationPlayer.h"


Player::Player() : Entity() {
    Mover* mover = new Mover(this);
    AddComponent(mover);
    Model* model = new Model(this);
    AddComponent(model);
    AnimationClip* clip = new AnimationClip(this);
    AddComponent(clip);
    AnimationPlayer* animPlayer = new AnimationPlayer(this);
    AddComponent(animPlayer);
}