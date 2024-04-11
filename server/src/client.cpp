#include "client.hpp"

void Client::init() {
    p = new Player;
}

void Client::handle_packet(void *packet) {
    std::string *cmd = new std::string((char *)packet);
    if (cmd->compare("forward") == 0) {
        p->inputs.y = 1;
    } else if (cmd->compare("back") == 0) {
        p->inputs.y = -1;
    } else if (cmd->compare("left") == 0) {
        p->inputs.x = -1;
    } else if (cmd->compare("right") == 0) {
        p->inputs.y = 1;
    }

    delete cmd;
}