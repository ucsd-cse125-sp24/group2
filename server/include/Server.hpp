#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <map>
#include <string>
#include <thread>

#include "Client.hpp"
#include "NetworkManager.hpp"
#include "psocket.hpp"

#define MAX_CLIENTS 8
#define SERVER_PORT 25565

class Server {
private:
    static Socket psocket;
    static void receive(Client*);

public:
    static std::map<int, Client*> clients;
    static int init();
    static int teardown();
    static int send(int, const char*, int);
};

#endif  // SERVER_H