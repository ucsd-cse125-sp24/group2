#ifndef SERVER_H
#define SERVER_H
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_CLIENTS 8

class Server {
    int max_clients;
    int num_connected_clients;

    private:
    public:
        int init();
};
#endif // SERVER_H