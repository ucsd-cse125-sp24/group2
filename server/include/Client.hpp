#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <string>

#include <winsock2.h>

#include "Player.hpp"

class Client {
private:
    static void* receive(void*);

public:
    Player* p;
    int id;
    int sockfd;
    struct sockaddr_in sockaddr;
    void handle_packet(void* packet);
    Client(int id, int sockfd, struct sockaddr_in& sockaddr) {
        this->id = id;
        this->sockaddr = sockaddr;
        this->sockfd = sockfd;
    }
    void init();
};

#endif  // CLIENT_H