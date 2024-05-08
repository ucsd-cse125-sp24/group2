#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include <glm/glm.hpp>
#include <map>
#include "Packet.hpp"
#include "EventArgs.hpp"
#include "Event.hpp"
#include <thread>
#include "Player.h"
#include "engine/Scene.hpp"

class GameManager {

public:
    Scene scene;
    std::map<int, Player*> players;
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