#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <pthread.h>
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

#endif // CLIENT_H