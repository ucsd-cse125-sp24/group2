#include "NetworkManager.hpp"

#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>
#include <algorithm>
#include <functional>
#include <algorithm>

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
    // Setup event handlers
    /* Can alternatively use lambda, like this:
        server.set_client_joined_callack([this](const EventArgs* a) {
            ClientJoinedEventArgs* args = (ClientJoinedEventArgs*)a;
            ...
        });
    */
    auto msg_received_callback = std::bind(&NetworkManager::on_message_received,
                                           this, std::placeholders::_1);
    server.set_message_received_callback(msg_received_callback);
    auto client_joined_callback = std::bind(&NetworkManager::on_client_joined,
                                            this, std::placeholders::_1);
    server.set_client_joined_callback(client_joined_callback);

    // Register listener for object added
    scene.object_added += [this](EventArgs* e) {
        ObjectEventArgs* args = (ObjectEventArgs*)e;

        std::vector<Client*> clients = server.get_clients();
        for (auto it : clients) {
            if (it->clientsock == nullptr)
                continue;
            it->track_object(args->e);
        }
    };
    scene.object_removed +=
        [this](EventArgs* e) { ObjectEventArgs* args = (ObjectEventArgs*)e; };

    // Start server
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
        packet->read_int(&packet_type);

        // but for now, we do this to set input manually
        switch ((PacketType)packet_type) {
        case PacketType::PLAYER_INPUT:
            char input[4];
            for (int i = 0; i < 4; i++) {
                packet->read_byte(&input[i]);
            }
            server.get_clients()[client_id]->p->inputs.x =
                (float)input[3] - (float)input[1];
            server.get_clients()[client_id]->p->inputs.y =
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
    std::vector<Client*> clients = server.get_clients();
    // Send all states to clients
    for (const auto& client : clients) {
        // Serialize all network objects into single state update packet
        Packet* updates = new Packet();
        updates->write_int((int)PacketType::STATE_UPDATE);
        updates->write_int(scene.entities.size());
        for (const auto& obj : scene.entities) {
            obj->serialize(updates);
        }
        server.send(client->id, updates);
    }
}

// FIXME handle incomplete packets or multiple packets per send() call
// thread-safe
void NetworkManager::on_message_received(const EventArgs* e) {
    MessageReceivedEventArgs* args = (MessageReceivedEventArgs*)e;

    // Create new packet from received data
    Packet* packet = new Packet();
    packet->write((uint8_t*)args->buffer, args->size);

    std::lock_guard<std::mutex> lock(_mutex);
    message_queue.push_back(std::pair<int, Packet*>(args->fromClient, packet));
}

void NetworkManager::on_client_joined(const EventArgs* e) {
    ClientJoinedEventArgs* args = (ClientJoinedEventArgs*)e;

    // Give client control over player
    Player* p = new Player();
    server.clients[args->clientId]->p = p;
    // Create player model
    scene.add_object(p);
}