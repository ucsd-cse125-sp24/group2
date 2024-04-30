#ifndef CLIENT_H
#define CLIENT_H

#include "psocket.hpp"
#include <stdio.h>
#include <string>

#include "Player.hpp"

class Client {
private:
    static void* receive(void*);

public:
    Player* p;
    int id;
    Socket* clientsock;
    struct sockaddr_in sockaddr;
    Client(int id) : id(id) {
        clientsock = nullptr;
    }
    void init();
    void disconnect();
};

#endif // CLIENT_H