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
        PlayerManager* p = new PlayerManager();
        p->id = player_id;
        p->mover = nullptr;
        players[player_id] = p;
    }

    glm::vec3 new_pos;
    pkt->read_vec3(&new_pos);

    // FIXME make thread safe
    if (players[player_id]->mover != nullptr)
        players[player_id]->mover->position = new_pos;
}