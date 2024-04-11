#ifndef CLIENT_H
#define CLIENT_H

#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <string>

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include "player.hpp"

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