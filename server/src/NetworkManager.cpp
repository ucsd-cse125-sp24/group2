#include "NetworkManager.hpp"

#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>
#include <functional>

#include "Server.hpp"
#include "Scene.hpp"

#define MAX_NETWORK_OBJECTS 4096

Server server;
Scene scene;
union FloatUnion {
    float f;
    uint32_t l;
} num;
void NetworkManager::init() {
    server.set_message_received_callback([this](const EventArgs* e) {
        MessageReceivedEventArgs* args = (MessageReceivedEventArgs*)e;
        handle_packet(args->fromClient, args->buffer);
    });
    server.set_client_joined_callback([this](const EventArgs* e) {
        ClientJoinedEventArgs* args = (ClientJoinedEventArgs*)e;

        // Give client control over player
        Player* p = new Player();
        server.clients[args->clientId]->p = p;
        // Create player model
        scene.add_object(p);
        networkObjects.push_back(p);
    });
    std::thread(&Server::start, &server).detach();
}

void NetworkManager::process_input() {
    std::lock_guard<std::mutex> lock(_mutex);
    while (!message_queue.empty()) {
        std::pair<int, Packet*> msg = message_queue.front();
        message_queue.pop_front();
        int client_id = msg.first;
        Packet* packet = msg.second;

        // TODO create handlers for each packet type
        int packet_type;
        /*
        for (int i = 0; i < 8; i++) {
            std::cout << std::setfill('0') << std::setw(2) << std::hex
                      << (int)packet->getBytes()[i] << " ";
        }
        std::cout << std::endl;
        */
        packet->read_int(&packet_type);

        // but for now, we do this to set input manually
        switch ((PacketType)packet_type) {
        case PacketType::PLAYER_INPUT:
            char input[4];
            for (int i = 0; i < 4; i++) {
                packet->read_byte(&input[i]);
            }
            (*server.get_clients())[client_id]->p->inputs.x =
                (float)input[3] - (float)input[1];
            (*server.get_clients())[client_id]->p->inputs.y =
                (float)input[0] - (float)input[2];
            break;
        default:
            break;
        }
    }
}

void NetworkManager::update() { scene.update(); }

// TODO send state of all networked entities
void NetworkManager::send_state() {
    uint8_t buf[12];

    std::vector<Client*>* clients = server.get_clients();
    // Send all states to clients
    for (const auto& client : *clients) {
        // Skip disconnected clients
        if (client->clientsock == nullptr)
            continue;

        // Serialize all network objects into single state update
        Packet* updates = new Packet();
        updates->write_int((int)PacketType::STATE_UPDATE);
        updates->write_int(networkObjects.size());
        for (const auto& obj : networkObjects) {
            client->track_object(obj);
            obj->serialize(updates);
        }
        server.send(client->id, updates);
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