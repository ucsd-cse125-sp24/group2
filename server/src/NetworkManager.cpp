#include "NetworkManager.hpp"

#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>
#include <algorithm>
#include <functional>

#include "Server.hpp"

int NetworkManager::next_network_id = 0;
Server server;
union FloatUnion {
    float f;
    uint32_t l;
} num;
void NetworkManager::init() {
    server.set_callback(
        [this](int client_id, void* pkt) { handle_packet(client_id, pkt); });
    std::thread(&Server::start, &server).detach();
}

void NetworkManager::process_input() {
    std::lock_guard<std::mutex> lock(_mutex);
    while (!message_queue.empty()) {
        std::pair<int, Packet*> msg = message_queue.front();
        message_queue.pop_front();
        int client_id = msg.first;
        Packet* packet = msg.second;

        // TODO call appropriate handler based on packet type
        PacketType packet_type;
        packet->read_int((int*)&packet_type);

        // but for now, we do this to set input manually
        char input[4];
        for (int i = 0; i < 4; i++) {
            packet->read_byte(&input[i]);
        }
        (*server.get_clients())[client_id]->p->inputs.x =
            (float)input[3] - (float)input[1];
        (*server.get_clients())[client_id]->p->inputs.y =
            (float)input[0] - (float)input[2];
    }
}

void NetworkManager::update() {
    for (int i = 0; i < entities.size(); i++) {
        entities[i]->update();
    }
}

// TODO send all networked entities
void NetworkManager::send_state() {
    uint8_t buf[12];

    std::vector<Client*>* clients = server.get_clients();
    // For some player p
    for (const auto& it0 : *clients) {
        if (it0->clientsock == nullptr)
            continue;
        // Send player p's position to all clients
        for (const auto& it : *clients) {
            if (it->clientsock == nullptr)
                continue;

            // Write packet type
            Packet* packet = new Packet();
            packet->write_int((int)PacketType::PLAYER_POSITION);

            // Write player id
            packet->write_int(it0->id);

            // Write player position
            auto tmp = it0->p->position;
            uint32_t tmpl;
            num.f = tmp.x;
            packet->write_int(num.l);
            num.f = tmp.y;
            packet->write_int(num.l);
            num.f = tmp.z;
            packet->write_int(num.l);

            server.send(it->id, packet);
        }
    }
}

// FIXME handle incomplete packets or multiple packets per send() call
// thread-safe
void NetworkManager::handle_packet(int client_id, void* pkt) {
    // Create new packet from received data
    Packet* packet = new Packet();
    packet->write((uint8_t*)pkt, 8);

    std::lock_guard<std::mutex> lock(_mutex);
    message_queue.push_back(std::pair<int, Packet*>(client_id, packet));
}

void NetworkManager::register_entity(Entity* e) {
    e->network_id = next_network_id++;
    entities.push_back(e);
}

void NetworkManager::unregister_entity(Entity* e) {
    entities.erase(std::remove(entities.begin(), entities.end(), e),
                   entities.end());
}