#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <map>
#include <string>
#include <thread>
#include <winsock2.h>

#include "Client.hpp"
#include "NetworkManager.hpp"

#define MAX_CLIENTS 8
#define SERVER_PORT 25565

class Server {
#ifdef _WIN32
    static WSADATA wsa_data;
#endif

   private:
    static void receive(Client*);

   public:
    static std::map<int, Client*> clients;
    static int init();
    static int teardown();
    static int send(int, const char*, int);
};

#endif  // SERVER_H