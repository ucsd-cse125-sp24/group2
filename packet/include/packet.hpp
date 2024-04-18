#ifndef PACKET_H
#define PACKET_H
#include <stdio.h>
#include <iostream>
#include "glm/glm.hpp"
#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#endif

class Packet {
#ifdef _WIN32
    WSADATA wsa_data;
#endif

   public:
    std::vector<u_int8_t> buffer;
    //int buffer_position;
    //type packet_type;

    Packet();
    ~Packet();
    void write_byte(char data);
    void write_int(int data);
    void write_double(double data);
    void write_vec3(glm::vec3 data);
    char read_byte();
    int read_int();
    double read_double();
    glm::vec3 read_vec3();

    int32_t* getBytes();
};

#endif  // PACKET_H