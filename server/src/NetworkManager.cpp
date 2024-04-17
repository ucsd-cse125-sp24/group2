#include "NetworkManager.hpp"

#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>
#include <functional>

#include "Server.hpp"

Server server;
union FloatUnion {
    float f;
    uint32_t l;
} num;
void NetworkManager::init() {
    server.set_callback([this](void* pkt) { handle_packet(pkt); });
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

void NetworkManager::handle_packet(void* pkt) {
    // FIXME free pkt
    uint8_t* packet = (uint8_t*)pkt;
    (*server.get_clients())[0]->p->inputs.x =
        (float)packet[3] - (float)packet[1];
    (*server.get_clients())[0]->p->inputs.y =
        (float)packet[0] - (float)packet[2];
    std::cout << std::endl;
}

void NetworkManager::register_entity(Entity* e) { entities.push_back(e); }