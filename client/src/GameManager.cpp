#include "GameManager.hpp"
#include "ColorCodes.hpp"
#include <thread>
#include "components/RendererComponent.hpp"
#include "components/Model.h"
#include <AudioManager.hpp>
#include "prefabs/Enemy.hpp"
#include <algorithm>
#include "AssetManager.hpp"
#include "components/PlayerComponent.hpp"
#include "HUD.h"

const std::string path = "../assets/robot/robot.gltf";
const std::string enemyPath = "../assets/donut-042524-02/donut.gltf";
const std::string robotPath = "../assets/robot/robot.gltf";
const std::string smallRobotPath = "../assets/small_robot/robot.gltf";
void StartGame(Packet*);

int localPlayerObject = -1;
void GameManager::Init() {}

void GameManager::handle_packet(Packet* packet) {
    int packet_id;
    packet->read_int(&packet_id);
    PacketType p = (PacketType)packet_id;

    // std::cout << "HERE GM::handle" << std::endl;
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
    // std::cout << "HERE GM::upd()" << std::endl;
    while (num_updates) {
        NetworkObjectTypeID _typeid;
        pkt->read_int((int*)&_typeid);

        // TODO deserialize
        switch (_typeid) {
        case NetworkObjectTypeID::ENEMY: {
            break;
        }
        case NetworkObjectTypeID::PLAYER: {
            // std::cout << "    ObjTypeID: Player" << std::endl;
            int network_id;
            pkt->read_int(&network_id);
            // Could not find object, create it
            if (players.find(network_id) == players.end()) {
                Player* playerPrefab = new Player(smallRobotPath, network_id);
                std::vector<AnimationClip*> prefabClips =
                    AssetManager::Instance().GetClips(smallRobotPath);
                for (int i = 0; i < prefabClips.size(); ++i) {
                    AnimationClip* clip = new AnimationClip(prefabClips[i]);
                    // std::cout << "Adding clip: " << clip->getName()
                    //           << std::endl;
                    playerPrefab->GetComponent<AnimationPlayer>()->AddClip(
                        clip);
                }

                players[network_id] = playerPrefab;
                scene.Instantiate(playerPrefab);

                if (players.size() == 2) {
                    Packet* pkt = new Packet();
                    pkt->write_int((int)PacketType::CLIENT_READY);
                    client.send(pkt);
                }
                if (localPlayerObject == network_id) {
                    std::cout << "HERE" << std::endl;
                    HUDs* hudComponent = new HUDs(playerPrefab);
                    playerPrefab->AddComponent(hudComponent);
                }

                /* adds the rest of the players to the local player's teamInfo
                 * if local player exists */
                if (players.find(localPlayerObject) != players.end()) {
                    players[localPlayerObject]
                        ->GetComponent<HUDs>()
                        ->enableState(VISIBLE);
                    for (auto it = players.begin(); it != players.end(); it++) {
                        auto map = players[localPlayerObject]
                                       ->GetComponent<HUDs>()
                                       ->teamInfo->teamHealthMap;
                        // std::cout<<"net id: " << it->first << std::endl;
                        if (it->first != localPlayerObject &&
                            map.find(it->first) == map.end()) {
                            std::cout << "add player " << it->first << " to "
                                      << "player " << localPlayerObject
                                      << std::endl;
                            players[localPlayerObject]
                                ->GetComponent<HUDs>()
                                ->teamInfo->addTeamMember(it->first);
                        }
                    }
                }
            }
            players[network_id]->deserialize(pkt);

            cam->SetTarget(glm::vec3(0, 0, 0));
            if (localPlayerObject == network_id) {
               
                auto playerPos = players[localPlayerObject]
                                     ->GetComponent<NetTransform>()
                                     ->position;
                // std::cout<<"player's pos :" << glm::to_string(playerPos)<<std::endl;
                auto playerRightVector = glm::normalize(glm::cross(
                    cam->GetTarget() - playerPos, glm::vec3(0, 1, 0)));
            
                cam->SetPosition(
                    playerPos +
                    glm::normalize(playerPos - cam->GetTarget()) * 3.0f +
                    glm::vec3(0, 2.0f, 0) + playerRightVector * 1.0f);
            }

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
    AudioManager::instance().Play();
    AudioManager::instance().GoToNextAudioPhase();

    // set the BPM once it plays the music
    players[localPlayerObject]->GetComponent<HUDs>()->metronome->setBpm(
        AudioManager::instance().getBpm());
}
