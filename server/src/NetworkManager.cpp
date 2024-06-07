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
#include "NetworkObjectState.hpp"
#include "ColorCodes.hpp"
#include "PlayerCombat.hpp"
#include "Enemy.hpp"
#include "Mover.hpp"
#include "AttackManager.hpp"
#include "Health.hpp"
#include "GameState.hpp"
#include "CooldownComponent.hpp"
#include "MovementStateMachine.hpp"

#define MAX_NETWORK_OBJECTS 4096

Server server;
bool isServerReady = false;
int playersReady = 0;
Enemy* enemyPrefab;
std::vector<glm::vec3> spawnPoints = {glm::vec3(60, 0, 0), glm::vec3(0, 0, 60),
                                      glm::vec3(-60, 0, 0),
                                      glm::vec3(0, 0, -60)};
int spawnIndex = 0;
union FloatUnion {
    float f;
    uint32_t l;
} num;

GameState gameState = GameState::READY;

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
            char input[INPUT_LEN];
            for (int i = 0; i < INPUT_LEN; i++) {
                packet->read_byte(&input[i]);
            }

            std::map<int, Client*> clients = server.get_clients();
            if (clients.find(client_id) == clients.end())
                break;

            // std::cout << "  Received input: " << (float)input[0] << ", "
            //           << (float)input[1] << ", " << (float)input[2] << ", "
            //           << (float)input[3] << std::endl;

            Player* currPlayer = clients[client_id]->p;
            Mover* currMover = currPlayer->GetComponent<Mover>();

            for (int i = 0; i < INPUT_LEN; ++i) {
                currMover->inputs[i] = input[i];
            }
            currMover->input.x = (float)input[3] - (float)input[1];
            currMover->input.y = (float)input[0] - (float)input[2];

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
            float judgment;
            packet->read_float(&judgment);
            // printf("  judgment: %d\n", judgment);
            std::map<int, Client*> clients = server.get_clients();
            // TODO set miss time to variable
            if (abs(judgment) > 0.3f) {
                clients[client_id]
                    ->p->GetComponent<PlayerCombat>()
                    ->ResetAllCombos();
                printf(RED "MISSED\n" RST);
                send_combo(client_id, 0);
                break;
            }

            std::vector<int> comboSeq =
                clients[client_id]->p->GetComponent<PlayerCombat>()->CheckCombo(
                    key);
            if (!comboSeq.empty()) {
                printf(YLW "COMBO HIT\n" RST);

                if (comboSeq == clients[client_id]->p->attack1 ||
                    comboSeq == clients[client_id]->p->attack2) {
                    AttackManager::instance().newPlayerAttack(
                        clients[client_id]->p);
                }

                if (comboSeq == clients[client_id]->p->heal) {
                    AttackManager::instance().newPlayerHeal(
                        clients[client_id]->p);
                }

                if (comboSeq == clients[client_id]->p->revive) {
                    AttackManager::instance().newPlayerRevive(
                        clients[client_id]->p);
                }

                if (comboSeq == clients[client_id]->p->speedBoost) {
                    AttackManager::instance().newPlayerSpeedUp(
                        clients[client_id]->p);
                }
            }

            break;
        }

        case PacketType::CLIENT_READY:
            playersReady++;

            if (playersReady == MAX_CLIENTS) {
                gameState = GameState::START;
                // TODO Spawn enemy
                // network enemy here
                printf("Spawn enemy!\n");
                enemyPrefab = new Enemy();
                enemyPrefab->GetComponent<NetTransform>()->SetPosition(
                    glm::vec3(0, 0, 0));
                AttackManager::instance().addEnemy(enemyPrefab);
                scene.Instantiate(enemyPrefab);

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
    // AttackManager update goes first so that whatever needs to be destroyed
    // gets destroyed in the next tick, to account for effects that happen
    // instantaneously
    AttackManager::instance().update(deltaTime);
    scene.Update(deltaTime);
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

    // maybe also keep track of game state now
    if (gameState == GameState::START &&
        (enemyPrefab->GetComponent<Health>()->GetHealth() <= 0 ||
         numAlive == 0)) {
        // i hope this doesn't get called before any clients connect.
        gameState = numAlive > 0 ? GameState::WIN : GameState::LOSE;
        for (const auto& kv : clients) {
            Client* client = kv.second;
            Packet* endGame = new Packet();
            endGame->write_int((int)PacketType::END_GAME);
            endGame->write_int((int)gameState);
            server.send(client->id, endGame);
        }
    }
}

void NetworkManager::send_next_phase() {
    std::map<int, Client*> clients = server.get_clients();

    for (const auto& kv : clients) {
        Client* client = kv.second;
        Packet* nextPhase = new Packet();
        nextPhase->write_int((int)PacketType::NEXT_PHASE);
        server.send(client->id, nextPhase);
    }
}

void NetworkManager::send_combo(int clientId, int comboIndex) {
    Packet* p = new Packet();
    p->write_int((int)PacketType::COMBO_INDEX);
    p->write_int(comboIndex);
    server.send(clientId, p);
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
    glm::vec3 position = spawnPoints[spawnIndex++ % spawnPoints.size()];
    Player* p = new Player(position, args->clientId);
    server.clients[args->clientId]->p = p;
    AttackManager::instance().addPlayer(p);
    numAlive++;

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