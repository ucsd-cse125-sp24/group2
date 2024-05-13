#include "GameManager.hpp"
#include "ColorCodes.hpp"
#include <thread>
#include "components/RendererComponent.hpp"
#include "components/Model.h"
#include <AudioManager.hpp>
#include "prefabs/Enemy.hpp"

union FloatUnion {
    float f;
    uint32_t l;
} num;

const std::string path = "../assets/animation/model.gltf";
const std::string enemyPath = "../assets/donut-042524-02/donut.gltf";

void StartGame(Packet*);

int localPlayerObject = -1;
void GameManager::Init() {
    model = new Model(path, true);
    enemy = new Model(enemyPath, false);
}

void GameManager::handle_packet(Packet* packet) {
    int packet_id;
    packet->read_int(&packet_id);
    PacketType p = (PacketType)packet_id;

    switch (p) {
    case PacketType::STATE_UPDATE:
        update(packet);
        break;
    case PacketType::DESTROY_OBJECT:
        destroy_object(packet);
        break;
    case PacketType::SET_LOCAL_PLAYER:
        int netId;
        packet->read_int(&netId);
        localPlayerObject = netId;
        break;
    case PacketType::SERVER_READY: {
        break;
    }
    case PacketType::START_GAME:
        StartGame(packet);
        break;

    default:
        break;
    }
}

void GameManager::update(Packet* pkt) {
    int num_updates;
    pkt->read_int(&num_updates);

    while (num_updates) {
        NetworkObjectTypeID _typeid;
        pkt->read_int((int*)&_typeid);

        // TODO deserialize
        switch (_typeid) {
        case NetworkObjectTypeID::PLAYER: {
            int network_id;
            pkt->read_int(&network_id);
            // Could not find object, create it
            if (players.find(network_id) == players.end()) {
                Player* playerPrefab = new Player();
                AnimationPlayer* animationPlayer =
                    new AnimationPlayer(path, model);
                RendererComponent* meshRenderer =
                    new RendererComponent(playerPrefab, ShaderType::ANIMATED);
                playerPrefab->AddComponent(model);
                playerPrefab->AddComponent(animationPlayer);
                playerPrefab->AddComponent(meshRenderer);
                players[network_id] = playerPrefab;

                scene.Instantiate(playerPrefab);

                if (players.size() == 4) {
                    Packet* pkt = new Packet();
                    pkt->write_int((int)PacketType::CLIENT_READY);
                    client.send(pkt);
                }
            }

            pkt->read_int((int*)&num.l);
            float x = num.f;
            pkt->read_int((int*)&num.l);
            float y = num.f;
            pkt->read_int((int*)&num.l);
            float z = num.f;

            players[network_id]->position = glm::vec3(x, y, z);
            if (localPlayerObject == network_id) {
                cam->position = glm::vec3(x, y + 200, z + 500);
            }
            break;
        }
        default:
            break;
        }

        num_updates--;
    }
}

void GameManager::destroy_object(Packet* pkt) {
    int numObjectsToDestroy;
    pkt->read_int(&numObjectsToDestroy);
    std::vector<int> objIdsDestroyed;
    while (numObjectsToDestroy) {
        int objIdToDestroy;
        pkt->read_int(&objIdToDestroy);

        // Found object, destroy it
        if (players.find(objIdToDestroy) != players.end()) {
            printf(RED "DESTROYING OBJECT\n" RST);
            scene.Destroy(players[objIdToDestroy]);
            delete players[objIdToDestroy];
            players.erase(objIdToDestroy);

            objIdsDestroyed.push_back(objIdToDestroy);
        }
        numObjectsToDestroy--;
    }

    Packet* destroyed_ack = new Packet();
    destroyed_ack->write_int((int)PacketType::DESTROY_OBJECT_ACK);
    destroyed_ack->write_int(objIdsDestroyed.size());
    for (int destroyedObjId : objIdsDestroyed) {
        destroyed_ack->write_int(destroyedObjId);
    }
    client.send(destroyed_ack);
}

void GameManager::StartGame(Packet* packet) {
    printf(GRN "Starting game!\n" RST);
    AudioManager::instance().play();

    Enemy* enemyPrefab = new Enemy();
    enemyPrefab->AddComponent(enemy);
    RendererComponent* meshRenderer =
        new RendererComponent(enemyPrefab, ShaderType::STANDARD);
    enemyPrefab->AddComponent(meshRenderer);

    scene.Instantiate(enemyPrefab);
}