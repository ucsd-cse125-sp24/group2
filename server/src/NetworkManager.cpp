#include "NetworkManager.hpp"

#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>

#include "Server.hpp"

Server server;
union FloatUnion {
    float f;
    uint32_t l;
} num;
void NetworkManager::init() { 
    std::thread(&Server::start, &server).detach();
}

void NetworkManager::update() {
    for (int i = 0; i < entities.size(); i++) {
        entities[i]->update();
    }
}

void NetworkManager::send_state() {
    uint8_t buf[12];
    for (int i = 0; i < entities.size(); i++) {
        std::vector<Client*>* clients = server.get_clients();
        for (const auto& it : *clients) {
            memset(buf, 0, 12);
            auto tmp = entities[i]->position;

            uint32_t tmpl;
            num.f = tmp.x;
            tmpl = htonl(num.l);
            memcpy(buf, &tmpl, sizeof(uint32_t));

            num.f = tmp.y;
            tmpl = htonl(num.l);
            memcpy(buf + 4, &tmpl, sizeof(uint32_t));

            num.f = tmp.z;
            tmpl = htonl(num.l);
            memcpy(buf + 8, &tmpl, sizeof(uint32_t));
            server.send(it->id, (const char*)buf, 12);
        }
        delete clients;
    }
}

void NetworkManager::register_entity(Entity* e) { entities.push_back(e); }