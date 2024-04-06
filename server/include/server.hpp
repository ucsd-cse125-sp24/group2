#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_CLIENTS 8
#define SERVER_PORT 25565

class Server {
    int max_clients;
    int num_connected_clients;
    WSADATA wsa_data;

   private:
   public:
    Server();
    ~Server();
    int init();
};
#endif  // SERVER_H
