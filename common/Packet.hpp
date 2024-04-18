#ifndef PACKET_H
#define PACKET_H
#include "glm/glm.hpp"
#include <vector>

class Packet {
public:
    std::vector<uint8_t> buffer;
    // int buffer_position;
    // type packet_type;

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

    uint8_t* getBytes();
};

#endif // PACKET_H