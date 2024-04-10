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

#include "player.hpp"
#include "client.hpp"

#pragma comment(lib, "ws2_32.lib")

#define MAX_CLIENTS 8
#define SERVER_PORT 25565

class Server {
    int num_connected_clients;
#ifdef _WIN32
    WSADATA wsa_data;
#endif

   private:
    Player player;
    std::map<int, Client*> clients;
    static void* receive(void*);
    void handle_packet(void*);

   public:
    Server();
    ~Server();
    int init();
    int send(void*);
};
#endif  // SERVER_H
