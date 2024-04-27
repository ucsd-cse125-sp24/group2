#ifndef PACKET_H
#define PACKET_H
#include "glm/glm.hpp"
#include <string.h>
#include <deque>

class Packet {

   public:
    std::deque<uint8_t> buffer;

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

    uint8_t* getBytes();
};

#endif  // PACKET_H