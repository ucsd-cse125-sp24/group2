#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include "Mover.h"
class Client {
    private:
        WSAData wsa_data;
        SOCKET sock = INVALID_SOCKET;
    public:
        void init(Mover*);
        static void receive(SOCKET, Mover*);
        void send(const char*, int);
};
#endif // CLIENT_H