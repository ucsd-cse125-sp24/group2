#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined __APPLE__
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#endif

#pragma comment(lib, "ws2_32.lib")

#define MAX_CLIENTS 8
#define SERVER_PORT 25565

class Server {
    int max_clients;
    int num_connected_clients;
#ifdef _WIN32
    WSADATA wsa_data;
#endif

   private:
    static void* receive(void*);

   public:
    Server();
    ~Server();
    int init();
};
#endif  // SERVER_H
