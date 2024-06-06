#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include <glm/glm.hpp>
#include <map>
#include "Packet.hpp"
#include "EventArgs.hpp"
#include "Event.hpp"
#include <thread>
#include "Player.hpp"
#include "engine/Scene.hpp"
#include "Client.h"
#include "Camera.h"
#include "AnimationPlayer.h"

#define MAX_CLIENTS 1

class Model;
class GameManager {

public:
    Scene scene;
    Camera* cam;
    Client client;
    Model* model;
    Model* enemy;
    std::map<int, Player*> players;
    EventHandler<EventArgs> object_destroyed;

    static GameManager& instance() {
        static GameManager m;
        return m;
    }
    void Init();
    void handle_packet(Packet*);
    void update(Packet*);
    void destroy_object(Packet*);
    void StartGame(Packet*);
};
#endif