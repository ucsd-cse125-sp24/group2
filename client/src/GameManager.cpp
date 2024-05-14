#include "GameManager.hpp"
#include "ColorCodes.hpp"
#include <thread>

// union FloatUnion {
//     float f;
//     uint32_t l;
// } num;

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

    default:
        std::cout << "  PacketType: ERROR" << std::endl;
        break;
    }
}

void GameManager::update(Packet* pkt) {
    int num_updates;
    pkt->read_int(&num_updates);

    // std::cout << "    numUpd: " << num_updates << std::endl;

    while (num_updates) {
        NetworkObjectTypeID _typeid;
        pkt->read_int((int*)&_typeid);

        // TODO deserialize
        switch (_typeid) {
        case NetworkObjectTypeID::PLAYER: {
            // std::cout << "    ObjTypeID: Player" << std::endl;
            int network_id;
            pkt->read_int(&network_id);

            // Could not find object, create it
            if (players.find(network_id) == players.end()) {
                PlayerManager* p = new PlayerManager();
                p->id = network_id;
                p->player = new Player(
                    "../assets/male_basic_walk_30_frames_loop/scene.gltf");
                players[network_id] = p;
            }

            // float x, y, z;
            // pkt->read_float(&x);
            // pkt->read_float(&y);
            // pkt->read_float(&z);
            // // std::cout << "Received position: " << glm::to_string(glm::vec3(x, y, z)) << std::endl;
            // players[network_id]->mover->position = glm::vec3(x, y, z);
            
            players[network_id]->player->deserialize(pkt);
            
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

    // std::cout << "    NumDestroy: " << numObjectsToDestroy << std::endl;

    std::vector<int> objIdsDestroyed;
    while (numObjectsToDestroy) {
        int objIdToDestroy;
        pkt->read_int(&objIdToDestroy);

        // Found object, destroy it
        if (players.find(objIdToDestroy) != players.end()) {
            printf(RED "DESTROYING OBJECT\n" RST);
            // delete players[objIdToDestroy]->mover;
            // players[objIdToDestroy]->mover = nullptr;
            delete players[objIdToDestroy]->player;
            players[objIdToDestroy]->player = nullptr;
            players.erase(objIdToDestroy);

            objIdsDestroyed.push_back(objIdToDestroy);
        }
        numObjectsToDestroy--;
    }

    // FIXME have GameManager handle this as well
    DestroyedEventArgs* args = new DestroyedEventArgs(objIdsDestroyed);
    object_destroyed.invoke(args);
}