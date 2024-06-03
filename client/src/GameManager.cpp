#include "GameManager.hpp"
#include "ColorCodes.hpp"
#include <thread>
#include "components/RendererComponent.hpp"
#include "components/Model.h"
#include <AudioManager.hpp>
#include "Enemy.hpp"
#include <algorithm>
#include "AssetManager.hpp"
#include "components/PlayerComponent.hpp"
#include "GameState.hpp"
#include "HUD.h"

const std::string path = "../assets/robot/robot.gltf";
const std::string enemyPath = "../assets/Bear2/bear.gltf";
const std::string robotPath = "../assets/robot/robot.gltf";

void StartGame(Packet*);

int localPlayerObject = -1;
Enemy* boss;
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
    case PacketType::END_GAME:
        int win;
        packet->read_int(&win);
        if (win == (int)GameState::WIN) {
            // TODO: stop updating scene, move to win screen
            std::cout << "YOU WIN" << std::endl;
        } else if (win == (int)GameState::LOSE) {
            // TODO: stop update scene, move to lose scren
            std::cout << "GAME OVER" << std::endl;
        }
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
            int network_id;
            pkt->read_int(&network_id); // J: I did not thoroughly check if this is set correctly
            if (!boss) {
                boss = new Enemy(enemyPath, network_id);
                std::vector<AnimationClip*> prefabClips = 
                    AssetManager::Instance().GetClips(enemyPath);
                for (int i = 0; i < prefabClips.size(); i++) {
                    AnimationClip* clip = new AnimationClip(prefabClips[i]);
                    boss->GetComponent<AnimationPlayer>()->AddClip(clip);
                }

                scene.Instantiate(boss);
            }

            // cam->SetTarget(boss->GetComponent<NetTransform>()->GetPosition());

            break;
        }
        case NetworkObjectTypeID::PLAYER: {
            // std::cout << "    ObjTypeID: Player" << std::endl;
            int network_id;
            pkt->read_int(&network_id);
            // Could not find object, create it
            if (players.find(network_id) == players.end()) {
                Player* playerPrefab = new Player(robotPath, network_id);
                std::vector<AnimationClip*> prefabClips =
                    AssetManager::Instance().GetClips(robotPath);
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

            // TODO: set cam target to boss target
            cam->SetTarget(glm::vec3(0, 0, 0));
            if (localPlayerObject == network_id) {
                auto playerPos = players[localPlayerObject]
                                     ->GetComponent<NetTransform>()
                                     ->position;
                auto playerRightVector = glm::normalize(glm::cross(
                    cam->GetTarget() - playerPos, glm::vec3(0, 1, 0)));

                cam->SetPosition(
                    playerPos +
                    glm::normalize(playerPos - cam->GetTarget()) * 250.0f +
                    glm::vec3(0, 250, 0) + playerRightVector * 100.0f);
            }

            break;
        }
        case NetworkObjectTypeID::PLAYER_ATTACK: {
            std::cout << "    PlayerAttack" << std::endl;
            break;
        }
        case NetworkObjectTypeID::ENEMY_ATTACK: {
            std::cout << "    EnemyAttack" << std::endl;
            break;
        }
        default:
            std::cout << "    ObjTypeID: ERROR: " << _typeid << std::endl;
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