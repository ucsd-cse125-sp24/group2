#include "network_manager.hpp"

void NetworkManager::init() {
    Server::init();
}

void NetworkManager::update() {
    for (int i = 0; i < entities.size(); i++) {
        entities[i]->update();
    }
}

void NetworkManager::send_state() {
    for (int i = 0; i < entities.size(); i++) {
    }
}

void NetworkManager::register_entity(Entity* e) {
    entities.push_back(e);
}