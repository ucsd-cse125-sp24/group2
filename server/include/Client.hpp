#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <string>
#include "psocket.hpp"

#include "Player.hpp"

class Client {
   private:
    static void* receive(void*);

   public:
    Player* p;
    int id;
    Socket* clientsock;
    struct sockaddr_in sockaddr;
    void handle_packet(void* packet);
    Client(int id, Socket* clientsock, struct sockaddr_in& sockaddr) : id(id), clientsock(clientsock), sockaddr(sockaddr) {
    }
    void init();
};

#endif  // CLIENT_H