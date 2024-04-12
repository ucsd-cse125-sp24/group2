#include "Client.hpp"

void Client::init() {
    p = new Player;
}

void Client::handle_packet(void *packet) {
    char buf[4];
    memcpy(buf, packet, 4);

    p->inputs.y = (float) buf[0] - (float)buf[2];
    p->inputs.x = (float) buf[3] - (float)buf[1];
}