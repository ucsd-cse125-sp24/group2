#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include <vector>
#include "entity.hpp"
#include <stdio.h>
class GameManager {
    std::vector<Entity*> entities;

   public:
    void update();
    void send_state();
    void register_entity(Entity*);
    static GameManager& instance() {
        static GameManager s;
        return s;
    }
};

#endif // GAME_MANAGER_H