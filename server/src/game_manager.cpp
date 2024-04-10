#include "game_manager.hpp"

void GameManager::update() {
    for (int i = 0; i < entities.size(); i++) {
        entities[i]->update();
    }
}

void GameManager::send_state() {
    //perror("not implemented");
    return;

    for (int i = 0; i < entities.size(); i++) {
    }
}

void GameManager::register_entity(Entity* e) {
    entities.push_back(e);
}