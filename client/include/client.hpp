#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <winsock2.h>

class Client
{
    WSADATA wsa_data;

public:
    Client();
    ~Client();
    void init();
};

#endif // CLIENT_H