#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
class Client {
    private:
        WSAData wsa_data;
        SOCKET sock = INVALID_SOCKET;
    public:
        void init();
        static void receive(SOCKET);
        void send(const char*, int);
};
#endif // CLIENT_H