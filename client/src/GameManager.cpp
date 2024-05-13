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

    // std::cout << "HANDLEPKT:" << std::endl;

    switch (p) {
    case PacketType::STATE_UPDATE:
        // std::cout << "  RECV: STATE_UPD" << std::endl;
        update(packet);
        break;
    case PacketType::DESTROY_OBJECT:
        // std::cout << "  RECV: DESTROY" << std::endl;
        destroy_object(packet);
        break;

    default:
        // std::cout << "  RECV: NONEOFTHEABOVE" << std::endl;
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

            // TODO: implement in deserialization
            pkt->read_int((int*)&num.l);
            float x = num.f;
            pkt->read_int((int*)&num.l);
            float y = num.f;
            pkt->read_int((int*)&num.l);
            float z = num.f;
            // float x, y, z;
            // pkt->read_float(&x);
            // pkt->read_float(&y);
            // pkt->read_float(&z);

            // std::cout << "Received position: " << glm::to_string(glm::vec3(x, y, z)) << std::endl;

            players[network_id]->mover->position = glm::vec3(x, y, z);
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