#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include <glm/glm.hpp>
#include <map>
#include "PlayerManager.hpp"
#include "Packet.hpp"
#include "EventArgs.hpp"
#include "Event.hpp"
#include <thread>

class GameManager {

public:
    std::map<int, PlayerManager*> players;
    EventHandler<EventArgs> object_destroyed;

    static GameManager& instance() {
        static GameManager m;
        return m;
    }
    void handle_packet(Packet*);
    void update(Packet*);
    void destroy_object(Packet*);
};
#endif