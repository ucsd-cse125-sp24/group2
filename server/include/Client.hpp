#ifndef CLIENT_H
#define CLIENT_H

#include "psocket.hpp"
#include <stdio.h>
#include <string>
#include <map>

#include "Player.hpp"
#include "NetworkObjectState.hpp"

class Client {
private:
    std::map<int, NetworkObjectState> objectStates;
    static void* receive(void*);

public:
    Player* p;
    int id;
    Socket* clientsock;
    struct sockaddr_in sockaddr;

    Client(int id) : id(id) {}
    void init();
    void disconnect();
    void track_object(NetworkObject*);
};

#endif // CLIENT_H