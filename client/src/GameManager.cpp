#include "GameManager.hpp"
#include "ColorCodes.hpp"
#include <thread>
#include "components/RendererComponent.hpp"
#include "components/Model.h"

union FloatUnion {
    float f;
    uint32_t l;
} num;

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
                std::string path = "../assets/animation/scene.gltf";
                Model* model = new Model(path, true);
                // AnimationClip* clip = new AnimationClip(path, model);
                AnimationPlayer* animationPlayer =
                    new AnimationPlayer(path, model);
                RendererComponent* meshRenderer =
                    new RendererComponent(playerPrefab, ShaderType::ANIMATED);
                playerPrefab->AddComponent(model);
                // playerPrefab->AddComponent(clip);
                playerPrefab->AddComponent(animationPlayer);
                playerPrefab->AddComponent(meshRenderer);
                players[network_id] = playerPrefab;

                scene.Instantiate(playerPrefab);
            }

            pkt->read_int((int*)&num.l);
            float x = num.f;
            pkt->read_int((int*)&num.l);
            float y = num.f;
            pkt->read_int((int*)&num.l);
            float z = num.f;

            players[network_id]->position = glm::vec3(x, y, z);
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
    destroyed_ack->write_int(numObjectsToDestroy);
    for (int destroyedObjId : objIdsDestroyed) {
        destroyed_ack->write_int(destroyedObjId);
    }
    client.send(destroyed_ack);
}