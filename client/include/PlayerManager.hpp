#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H
#include <string>
#include "Player.h"

class PlayerManager {
public:
    int id;
    std::string username;
    // Mover* mover;

    Player* player;
};
#endif