#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include <glm/glm.hpp>
#include <map>
#include "Entity.h"
#include "Player.h"
//#include "PlayerManager.hpp"
#include "Packet.hpp"

#include "Mover.h" // here because GameManager currently needs to know about the Mover type
class GameManager {

public:
    // TODO initialize players
    std::map<int, Entity*> players;
    // std::map<int, PlayerManager*> players;
    static GameManager& instance() {
        static GameManager m;
        return m;
    }

    void handle_packet(Packet*);
    void player_position(Packet*);
};
#endif