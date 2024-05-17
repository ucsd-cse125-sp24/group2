#include "GameManager.hpp"
#include "ColorCodes.hpp"
#include <thread>
#include "components/RendererComponent.hpp"
#include "components/Model.h"
#include <AudioManager.hpp>
#include "prefabs/Enemy.hpp"
#include <algorithm>
#include "AssetManager.hpp"

const std::string path = "../assets/animation/model.gltf";
const std::string enemyPath = "../assets/donut-042524-02/donut.gltf";

void StartGame(Packet*);

int localPlayerObject = -1;
void GameManager::Init() {
    /*
    model = new Model(nullptr, path, true);
    enemy = new Model(nullptr, enemyPath, false);
    */
}

void GameManager::handle_packet(Packet* packet) {
    int packet_id;
    packet->read_int(&packet_id);
    PacketType p = (PacketType)packet_id;

    // std::cout << "GameManager::handle_packet():" << std::endl;

    switch (p) {
    case PacketType::STATE_UPDATE:
        // std::cout << "  PacketType: STATE_UPD" << std::endl;
        update(packet);
        break;
    case PacketType::DESTROY_OBJECT:
        // std::cout << "  PacketType: DESTROY" << std::endl;
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
        std::cout << "  PacketType: ERROR" << std::endl;
        break;
    }
}
int test = 0;
void GameManager::update(Packet* pkt) {
    int num_updates;
    pkt->read_int(&num_updates);
    while (num_updates) {
        NetworkObjectTypeID _typeid;
        pkt->read_int((int*)&_typeid);

        // TODO deserialize
        switch (_typeid) {
        case NetworkObjectTypeID::ENEMY: {
            /*
            int network_id;
            pkt->read_int(&network_id);
            auto it = std::find_if(scene.entities.begin(), scene.entities.end(),
                                   [network_id](Entity* entity) {
                                       return entity->networkId() == network_id;
                                   });
            Entity* enemyPrefab;
            if (it == scene.entities.end()) {
                enemyPrefab = new Enemy(network_id);
                enemyPrefab->AddComponent(enemy);
                RendererComponent* meshRenderer =
                    new RendererComponent(enemyPrefab, ShaderType::STANDARD);
                enemyPrefab->AddComponent(meshRenderer);
                scene.Instantiate(enemyPrefab);
            } else {
                enemyPrefab = *it;
            }

            enemyPrefab->deserialize(pkt);


            */
            break;
        }
        case NetworkObjectTypeID::PLAYER: {
            // std::cout << "    ObjTypeID: Player" << std::endl;
            int network_id;
            pkt->read_int(&network_id);
            // Could not find object, create it
            if (players.find(network_id) == players.end()) {
                Player* playerPrefab = new Player(path, network_id);
                std::vector<AnimationClip*> prefabClips = AssetManager::Instance().GetClips(path);
                for (int i = 0; i < prefabClips.size(); ++i) {
                    AnimationClip* clip = prefabClips[i];
                    std::cout << "Adding clip: " << clip->getName() << std::endl;
                    playerPrefab->GetComponent<AnimationPlayer>()->AddClip(clip);
                    if (i == 0) {
                        // std::cout << "PLAY THE DAMN CLIP" << std::endl;
                        playerPrefab->GetComponent<AnimationPlayer>()->play(clip);
                        // std::cout << "Now playing: " << playerPrefab->GetComponent<AnimationPlayer>()->currentAnimation->getName() << std::endl;
                    }
                }
                

                players[network_id] = playerPrefab;
                printf("player network id: %d\n",
                       players[network_id]->networkId());
                scene.Instantiate(playerPrefab);

                if (players.size() == 2) {
                    Packet* pkt = new Packet();
                    pkt->write_int((int)PacketType::CLIENT_READY);
                    client.send(pkt);
                }
            }

            players[network_id]->deserialize(pkt);
          
            cam->SetTarget(glm::vec3(0, 0, 0));
            if (localPlayerObject == network_id) {
                auto playerPos =
                    players[network_id]->GetComponent<NetTransform>()->position;
                auto playerRightVector = glm::normalize(glm::cross(
                    cam->GetTarget() - playerPos, glm::vec3(0, 1, 0)));

                cam->SetPosition(
                    playerPos +
                    glm::normalize(playerPos - cam->GetTarget()) * 250.0f +
                    glm::vec3(0, 250, 0) + playerRightVector * 100.0f);
            }

            // std::cout << "playinnn: " << players[network_id]->GetComponent<AnimationPlayer>()->currentAnimation->getName() << std::endl;

            break;
        }
        default:
            std::cout << "    ObjTypeID: ERROR" << std::endl;
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
}