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
        int object_id;
        pkt->read_int(&object_id);

        // Could not find object, create it
        if (players.find(object_id) == players.end()) {
            PlayerManager* p = new PlayerManager();
            p->id = object_id;
            p->mover = new Mover(
                "../assets/male_basic_walk_30_frames_loop/scene.gltf");
            players[object_id] = p;
        }

        pkt->read_int((int*)&num.l);
        float x = num.f;
        pkt->read_int((int*)&num.l);
        float y = num.f;
        pkt->read_int((int*)&num.l);
        float z = num.f;

        players[object_id]->mover->position = glm::vec3(x, y, z);
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