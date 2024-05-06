#include "GameManager.hpp"

union FloatUnion {
    float f;
    uint32_t l;
} num;

void GameManager::handle_packet(Packet* packet) {
    int packet_id;
    packet->read_int(&packet_id);
    PacketType p = (PacketType)packet_id;

    switch (p) {
    case PacketType::PLAYER_POSITION:
        player_position(packet);
        break;

    default:
        break;
    }
}

void GameManager::player_position(Packet* pkt) {
    int player_id;
    pkt->read_int(&player_id);

    if (players.find(player_id) == players.end()) {
        Player* p = new Player();
        players[player_id] = p;
    }

    pkt->read_int((int*)&num.l);
    float x = num.f;
    pkt->read_int((int*)&num.l);
    float y = num.f;
    pkt->read_int((int*)&num.l);
    float z = num.f;

    // FIXME make thread safe
    Mover* m = players[player_id]->GetComponent<Mover>();
    if (m != nullptr) {
        m->position = glm::vec3(x, y, z);
    }
}