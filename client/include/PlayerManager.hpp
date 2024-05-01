#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H
#include <string>
#include "Mover.h"
class PlayerManager {
public:
    int id;
    std::string username;
    Mover* mover;
};
#endif