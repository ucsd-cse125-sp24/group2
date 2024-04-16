#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <map>
#include <string>

#include "Client.hpp"
#include "NetworkManager.hpp"
#include "psocket.hpp"

#define MAX_CLIENTS 8
#define SERVER_PORT 25565

class Server {
private:
    std::map<int, Client*> clients;
    Socket psocket;
    void receive(Client* client);

public:
    void start();
    int teardown();
    int send(int, const char*, int);
    std::vector<Client*>* get_clients();
};

#endif  // SERVER_H