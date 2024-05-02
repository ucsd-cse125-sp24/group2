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
    static void* receive(void*);

public:
    std::map<int, NetworkObjectState> objectStates;
    Player* p;
    int id;
    Socket* clientsock;
    struct sockaddr_in sockaddr;
    Client(int id) : id(id) { clientsock = nullptr; }
    void init();
    void disconnect();
    void track_object(NetworkObject*);
    void mark_as_deleted(NetworkObject*);
};

#endif // CLIENT_H