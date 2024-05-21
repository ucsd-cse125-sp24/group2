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
#include "engine/Scene.hpp"
#include "NetworkObjectState.hpp"
#include "ColorCodes.hpp"
#include "PlayerCombat.hpp"
#include "prefabs/Enemy.hpp"
#include "Mover.hpp"
#include "AttackManager.hpp"

#define MAX_NETWORK_OBJECTS 4096

Server server;
Scene scene;
bool isServerReady = false;
int playersReady = 0;
std::vector<glm::vec3> spawnPoints = {
    glm::vec3(500, 0, 0), glm::vec3(0, 0, 500), glm::vec3(-500, 0, 0),
    glm::vec3(0, 0, -500)};
int spawnIndex = 0;
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
    server.set_client_disconnected_callback([this](EventArgs* e) {
        ClientEventArgs* args = (ClientEventArgs*)e;
        scene.Destroy(server.clients[args->clientId]->p);
    });

    // Register listener for object added
    scene.object_added += [this](EventArgs* e) {
        ObjectEventArgs* args = (ObjectEventArgs*)e;

        std::map<int, Client*> clients = server.get_clients();
        for (auto kv : clients) {
            Client* client = kv.second;
            client->track_object(args->e);
        }
    };
    scene.object_removed += [this](EventArgs* e) {
        ObjectEventArgs* args = (ObjectEventArgs*)e;
        std::map<int, Client*> clients = server.get_clients();
        for (auto kv : clients) {
            Client* client = kv.second;
            client->mark_as_deleted(args->e);
        }
    };

    // Start server
    std::thread(&Server::start, &server).detach();
}

void NetworkManager::process_input() {
    std::lock_guard<std::mutex> lock(_mutex);

    // std::cout << "Size of msgQ: " << message_queue.size() << std::endl;

    while (!message_queue.empty()) {
        std::pair<int, Packet*> msg = message_queue.front();
        message_queue.pop_front();
        int client_id = msg.first;
        Packet* packet = msg.second;

        // TODO create handlers for each packet type
        int packet_type;
        packet->read_int(&packet_type);

        /*
        printf("{\n");
        printf("  client: %d\n", client_id);
        printf("  packet_type: %d\n", packet_type);
        */

        // but for now, we do this to set input manually
        switch ((PacketType)packet_type) {
        case PacketType::PLAYER_INPUT: {
            char input[5];
            for (int i = 0; i < 5; i++) {
                packet->read_byte(&input[i]);
            }

            std::map<int, Client*> clients = server.get_clients();
            if (clients.find(client_id) == clients.end())
                break;

            // std::cout << "  Received input: " << (float)input[0] << ", "
            //           << (float)input[1] << ", " << (float)input[2] << ", "
            //           << (float)input[3] << std::endl;

            clients[client_id]->p->GetComponent<Mover>()->input.x =
                (float)input[3] - (float)input[1];
            clients[client_id]->p->GetComponent<Mover>()->input.y =
                (float)input[0] - (float)input[2];
            if (input[4] == 1) {
                clients[client_id]->p->GetComponent<Mover>()->speed = 9.0f;
            } else {
                clients[client_id]->p->GetComponent<Mover>()->speed = 4.0f;
            }

            break;
        }
        case PacketType::DESTROY_OBJECT_ACK: {
            // std::cout << "  RECV: DSTRY_ACK" << std::endl;
            int numObjectsDestroyed;
            packet->read_int(&numObjectsDestroyed);
            // printf("  numObjectsDestroyed: %d\n", numObjectsDestroyed);
            // printf("  destroyedObjectIds: [ ");
            while (numObjectsDestroyed) {
                int destroyedObjectId;
                packet->read_int(&destroyedObjectId);
                printf("%d ", destroyedObjectId);
                std::map<int, Client*> clients = server.get_clients();
                if (clients.size() != 0) {
                    clients[client_id]->objectStates.erase(destroyedObjectId);
                }

                numObjectsDestroyed--;
            }
            // printf("]\n");
            break;
        }
        case PacketType::PLAYER_ATTACK: {
            int key;
            packet->read_int(&key);
            // printf("  key: %d\n", key);
            int judgment;
            packet->read_int(&judgment);
            // printf("  judgment: %d\n", judgment);

            std::map<int, Client*> clients = server.get_clients();
            // TODO set miss time to variable
            if (abs(judgment) > 100) {
                clients[client_id]
                    ->p->GetComponent<PlayerCombat>()
                    ->ResetAllCombos();
                printf(RED "MISSED\n" RST);
                break;
            }

            if (clients[client_id]->p->GetComponent<PlayerCombat>()->CheckCombo(
                    key)) {
                printf(YLW "COMBO HIT\n" RST);
                // TODO enemy take damage
                AttackManager::instance().newPlayerAttack(clients[client_id]->p);
            }

            break;
        }

        case PacketType::CLIENT_READY:
            playersReady++;

            if (playersReady == MAX_CLIENTS) {
                // TODO Spawn enemy
                printf("Spawn enemy!\n");
                Enemy* enemyPrefab = new Enemy();
                enemyPrefab->GetComponent<NetTransform>()->position = glm::vec3(0, 0, 0);
                AttackManager::instance().addEnemy(enemyPrefab);
                // scene.Instantiate(enemyPrefab);

                // Start game for all players
                for (auto& kv : server.get_clients()) {
                    Packet* pkt = new Packet();
                    pkt->write_int((int)PacketType::START_GAME);
                    server.send(kv.first, pkt);
                }
            }
            break;
        default:
            // std::cout << "  RECV: NONEOFTHEABOVE" << std::endl;
            break;
        }
        // printf("}\n");
    }
}

void NetworkManager::update(float deltaTime) { 
    scene.Update(deltaTime); 
    AttackManager::instance().update(deltaTime);
}

// TODO send state of all networked entities
void NetworkManager::send_state() {
    std::map<int, Client*> clients = server.get_clients();
    // Send state updates to clients
    for (const auto& kv : clients) {
        Client* client = kv.second;
        // Serialize all network objects into single state update packet
        Packet* updates = new Packet();
        updates->write_int((int)PacketType::STATE_UPDATE);
        updates->write_int(scene.entities.size());
        for (const auto& obj : scene.entities) {
            updates->write_int(obj->TypeID());
            obj->serialize(updates);
        }
        server.send(client->id, updates);
    }

    // Send delete message to clients
    for (const auto& kv : clients) {
        Client* client = kv.second;
        Packet* destroy = new Packet();
        destroy->write_int((int)PacketType::DESTROY_OBJECT);

        int numObjectsToDestroy = 0;
        std::vector<int> objectIdsToDestroy;
        for (const auto& obj : client->objectStates) {
            if (obj.second.status & AWAITING_DESTRUCTION) {
                std::cout << "[NETWORK_MANAGER] Destroying object " << obj.first
                          << " on client " << client->id << std::endl;
                numObjectsToDestroy++;
                objectIdsToDestroy.push_back(obj.first);
            }
        }
        destroy->write_int(numObjectsToDestroy);
        for (int idToDestroy : objectIdsToDestroy) {
            destroy->write_int(idToDestroy);
        }
        server.send(client->id, destroy);
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
    ClientEventArgs* args = (ClientEventArgs*)e;

    // Give client control over player
    Player* p = new Player();
    server.clients[args->clientId]->p = p;
    // p->position = spawnPoints[spawnIndex++ % spawnPoints.size()];
    p->GetComponent<NetTransform>()->position =
        spawnPoints[spawnIndex++ % spawnPoints.size()];
    AttackManager::instance().addPlayer(p);

    Packet* pkt = new Packet();
    pkt->write_int((int)PacketType::SET_LOCAL_PLAYER);
    pkt->write_int(p->networkId());
    server.send(args->clientId, pkt);

    // Tell clients server is ready
    if (server.clients.size() == MAX_CLIENTS) {
        for (auto& kv : server.clients) {
            Packet* packet = new Packet();
            packet->write_int((int)PacketType::SERVER_READY);
            server.send(kv.first, packet);
        }
    }

    // Create player model
    scene.Instantiate(p);
}