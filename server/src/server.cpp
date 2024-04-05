#include "server.hpp"

int Server::init() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket() failed");
        return 1;
    }

    return 0;
}