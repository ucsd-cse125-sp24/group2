#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include <glm/glm.hpp>
#include <map>
#include "PlayerManager.hpp"
#include "Packet.hpp"
class GameManager {

public:
    // TODO initialize players
    std::map<int, PlayerManager*> players;
    static GameManager& instance() {
        static GameManager m;
        return m;
    }

    void handle_packet(Packet*);
    void player_position(Packet*);
};
#endif