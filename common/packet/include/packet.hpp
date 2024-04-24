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

   public:
    std::vector<u_int8_t> buffer;

    Packet();
    ~Packet();
    void write_byte(char data);
    void write_int(int data);
    void write_float(float data);
    void write_double(double data);
    void write_vec3(glm::vec3 data);

    // return -1 if error, else return size
    int read_byte(char* dest);
    int read_int(int* dest);
    int read_float(float* dest);
    int read_double(double* dest);
    int read_vec3(glm::vec3* dest);

    u_int8_t* getBytes();
};

#endif  // PACKET_H