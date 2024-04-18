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

void NetworkManager::process_input() {
    std::lock_guard<std::mutex> lock(_mutex);
    while (!message_queue.empty()) {
        std::cout << "dequeued a packet. size: " << message_queue.size()
                  << std::endl;
        Packet* packet = message_queue.front();
        message_queue.pop_front();

        // TODO call appropriate handler based on packet type
        int packet_type;
        packet->read_int(&packet_type);

        // but for now, we do this to set input manually
        char input[4];
        for (int i = 0; i < 4; i++) {
            packet->read_byte(&input[i]);
        }
        (*server.get_clients())[0]->p->inputs.x =
            (float)input[3] - (float)input[1];
        (*server.get_clients())[0]->p->inputs.y =
            (float)input[0] - (float)input[2];
    }
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

// FIXME handle incomplete packets or multiple packets per send() call
// thread-safe
void NetworkManager::handle_packet(void* pkt) {
    // Create new packet from received data
    Packet* packet = new Packet();
    packet->write((uint8_t*)pkt, 4);

    std::lock_guard<std::mutex> lock(_mutex);
    message_queue.push_back(packet);
}

void NetworkManager::register_entity(Entity* e) { entities.push_back(e); }