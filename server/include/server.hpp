#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <pthread.h>
#include <map>
#include <string>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined __APPLE__
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <glm/glm.hpp>
#endif

#include "client.hpp"
#include "network_manager.hpp"

#pragma comment(lib, "ws2_32.lib")

#define MAX_CLIENTS 8
#define SERVER_PORT 25565

class Server {
#ifdef _WIN32
    static WSADATA wsa_data;
#endif

   private:
    static void* receive(void*);

   public:
    static std::map<int, Client*> clients;
    static int init();
    static int teardown();
    static int send(int, void*, int);
};

#endif  // SERVER_H