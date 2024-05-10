#include "GameManager.hpp"
#include "ColorCodes.hpp"
#include <thread>

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
                PlayerManager* p = new PlayerManager();
                p->id = network_id;
                p->mover = new Mover(
                    "../assets/male_basic_walk_30_frames_loop/scene.gltf");
                players[network_id] = p;
            }

            glm::vec3 new_pos;
            pkt->read_vec3(&new_pos);

            // FIXME make thread safe
            if (players[network_id]->mover != nullptr) // TODO does l46 cover this?
                players[network_id]->mover->position = new_pos;

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
            delete players[objIdToDestroy]->mover;
            players[objIdToDestroy]->mover = nullptr;
            players.erase(objIdToDestroy);

            objIdsDestroyed.push_back(objIdToDestroy);
        }
        numObjectsToDestroy--;
    }
    // FIXME have GameManager handle this as well
    DestroyedEventArgs* args = new DestroyedEventArgs(objIdsDestroyed);
    object_destroyed.invoke(args);
}