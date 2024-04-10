#ifndef CLIENT_INFO_H
#define CLIENT_INFO_H

#include <stdio.h>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <pthread.h>

class Client {
    private:
        void handle_packet(void* packet);
        static void* receive(void*);
    public:
        int id;
        int sockfd;
        struct sockaddr_in sockaddr;
        Client(int id, int sockfd, struct sockaddr_in& sockaddr) {
            this->id = id;
            this->sockaddr = sockaddr;
            this->sockfd = sockfd;
        }
        void init();
};

#endif // CLIENT_INFO_H