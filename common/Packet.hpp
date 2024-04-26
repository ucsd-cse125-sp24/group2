#ifndef PACKET_H
#define PACKET_H
#include "glm/glm.hpp"
#include <deque>

enum class PacketType { PLAYER_POSITION, PLAYER_INPUT, PLAYER_ATTACK };
class Packet {
private:
    std::deque<uint8_t> buffer;

public:
    Packet();
    ~Packet();
    void write(uint8_t*, int);
    void write_byte(char data);
    void write_int(int data);
    void write_double(double data);
    void write_vec3(glm::vec3 data);
    void write_float(float data);
    int read_byte(char*);
    int read_int(int*);
    int read_double(double*);
    glm::vec3 read_vec3(glm::vec3*);

    uint8_t* getBytes();
    int size();
};

#endif // PACKET_H