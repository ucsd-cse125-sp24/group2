#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <vector>
#include <iostream>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#endif

using namespace std;
class Client {
#ifdef _WIN32
    WSADATA wsa_data;
#endif

   public:
    Client();
    ~Client();
    void init();
    static void* receive(void*);
};

#endif  // CLIENT_H