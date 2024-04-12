#include "NetworkManager.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <winsock2.h>

#include "Server.hpp"

union FloatUnion {
    float f;
    uint32_t l;
} num;
void NetworkManager::init() {
    Server::init();
}

void NetworkManager::update() {
    for (int i = 0; i < entities.size(); i++) {
        entities[i]->update();
    }
}

void NetworkManager::send_state() {
    uint8_t buf[12];
    for (int i = 0; i < entities.size(); i++) {
        for (const auto& it : Server::clients) {
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
            // printf("(%g, %g, %d)\n", tmp.x, tmp.y, tmpl);

            Server::send(it.first, (const char*) buf, 12);
        }
    }
}

void NetworkManager::register_entity(Entity* e) {
    entities.push_back(e);
}