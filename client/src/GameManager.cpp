#include "GameManager.hpp"
#include "ColorCodes.hpp"
#include <thread>
#include "components/RendererComponent.hpp"
#include "components/Model.h"
#include <AudioManager.hpp>
#include "Enemy.hpp"
#include <algorithm>
#include "AssetManager.hpp"
// #include "Status.hpp" // TODO: remove
#include "components/PlayerComponent.hpp"
#include "EnemyComponent.hpp"
#include "GameState.hpp"
#include "HUD.h"
#include "Health.hpp"

const std::string path = "../assets/robot/robot.gltf";
const std::string enemyPath = "../assets/bear_v5/bear.gltf";
const std::string robotPath = "../assets/robot/robot.gltf";

Enemy* boss = nullptr;
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
    case PacketType::NEXT_PHASE:
        AudioManager::instance().GoToNextAudioPhase();
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
    case PacketType::END_GAME: {
        int win;
        packet->read_int(&win);
        bool didWin = false;
        if (win == (int)GameState::WIN) {
            didWin = true;
            // TODO: stop updating scene, move to win screen
            std::cout << "YOU WIN" << std::endl;
        } else if (win == (int)GameState::LOSE) {
            didWin = false;
            // TODO: stop update scene, move to lose scren
            std::cout << "GAME OVER" << std::endl;
        }
        if (players.find(localPlayerObject) != players.end()) {
            players[localPlayerObject]
                ->GetComponent<HUDs>()
                ->endgame->drawVictory(didWin);
        }
        break;
    }
    case PacketType::COMBO_INDEX:
        int comboType;
        packet->read_int(&comboType);
        int index;
        packet->read_int(&index);
        if (players.find(localPlayerObject) != players.end()) {
            players[localPlayerObject]->GetComponent<HUDs>()->setComboCount(
                index);
            if (index == 4) {
                players[localPlayerObject]
                    ->GetComponent<HUDs>()
                    ->triggleHitText(comboType);
            }
        }
        printf("combo index: %d\n", index);
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
            // std::cout << "    ObjTypeID: Enemy" << std::endl;
            int network_id;
            pkt->read_int(&network_id); // J: I did not thoroughly check if the
                                        // packet is read correctly
            if (!boss) {
                boss = new Enemy(enemyPath, network_id);
                std::vector<AnimationClip*> prefabClips =
                    AssetManager::Instance().GetClips(enemyPath);
                for (int i = 0; i < prefabClips.size(); i++) {
                    AnimationClip* clip = new AnimationClip(prefabClips[i]);
                    boss->GetComponent<AnimationPlayer>()->AddClip(clip);
                }
                players[localPlayerObject]
                    ->GetComponent<HUDs>()
                    ->bossHealth->setMaxHealth(ENEMY_MAX_HEALTH);

                scene.Instantiate(boss);
            }

            boss->deserialize(pkt);
            players[localPlayerObject]
                ->GetComponent<HUDs>()
                ->bossHealth->enableState(VISIBLE);
            players[localPlayerObject]
                ->GetComponent<HUDs>()
                ->bossHealth->setHealth(
                    boss->GetComponent<Health>()->GetHealth());
            // also look up at the boss, probably needs to be the center of it
            // which is like 1000 or something rn
            glm::vec3 bossPos =
                boss->GetComponent<NetTransform>()->GetPosition();
            cam->SetTarget(glm::vec3(bossPos.x, 6.5f, bossPos.z));

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

            if (localPlayerObject == network_id) {

                for (int i = 0; i < players.size(); i++) {
                    if (i == localPlayerObject)
                        players[localPlayerObject]
                            ->GetComponent<HUDs>()
                            ->healthBar->setHealth(players[i]
                                                       ->GetComponent<Health>()
                                                       ->GetHealth());
                    else
                        players[localPlayerObject]
                            ->GetComponent<HUDs>()
                            ->teamInfo->teamHealthMap[i]
                            ->setHealth(players[i]
                                            ->GetComponent<Health>()
                                            ->GetHealth());
                }

                auto playerPos = players[localPlayerObject]
                                     ->GetComponent<NetTransform>()
                                     ->position;
                auto playerRightVector = glm::normalize(glm::cross(
                    cam->GetTarget() - playerPos, glm::vec3(0, 1, 0)));

                cam->basePosition =
                    playerPos +
                    glm::normalize(playerPos - cam->GetTarget()) * 5.0f +
                    glm::vec3(0, 2.0f, 0) + playerRightVector * 1.35f;
            }

            // std::cout <<
            // players[network_id]->GetComponent<Status>()->ToString() <<
            // std::endl;

            break;
        }
        case NetworkObjectTypeID::PLAYER_SKILL: {
            // std::cout << "    ObjTypeID: PlayerSkill" << std::endl;
            int network_id;
            pkt->read_int(&network_id);
            // Could not find object, create it
            if (playerSkills.find(network_id) == playerSkills.end()) {
                PlayerSkill* playerSkillPrefab = new PlayerSkill(network_id);
                playerSkillPrefab->deserialize(pkt);
                playerSkillPrefab->initComponent(
                    playerSkillPrefab->GetComponent<PlayerSkillType>()
                        ->GetState());
                playerSkills[network_id] = playerSkillPrefab;
                scene.Instantiate(playerSkillPrefab);
            } else {
                playerSkills[network_id]->deserialize(pkt);
            }

            break;
        }
        case NetworkObjectTypeID::ENEMY_ATTACK: {
            // std::cout << "    ObjTypeID: EnemyAttack" << std::endl;
            int network_id;
            pkt->read_int(&network_id);
            // Could not find object, create it
            if (enemyAttacks.find(network_id) == enemyAttacks.end()) {
                EnemyAttack* enemyAttackPrefab = new EnemyAttack(
                    boss->GetComponent<EnemyComponent>()->GetState(),
                    network_id);

                enemyAttacks[network_id] = enemyAttackPrefab;
                scene.Instantiate(enemyAttackPrefab);
                cam->Shake(0.15f, 0.4f, 0.25f);
            }

            enemyAttacks[network_id]->deserialize(pkt);

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
        if (playerSkills.find(objIdToDestroy) != playerSkills.end()) {
            // printf(RED "DESTROYING PLAYERSKILL\n" RST);
            scene.Destroy(playerSkills[objIdToDestroy]);
            delete playerSkills[objIdToDestroy];
            playerSkills.erase(objIdToDestroy);

            objIdsDestroyed.push_back(objIdToDestroy);
        }
        if (enemyAttacks.find(objIdToDestroy) != enemyAttacks.end()) {
            // printf(RED "DESTROYING ENEMYATTACK\n" RST);
            scene.Destroy(enemyAttacks[objIdToDestroy]);
            delete enemyAttacks[objIdToDestroy];
            enemyAttacks.erase(objIdToDestroy);

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
