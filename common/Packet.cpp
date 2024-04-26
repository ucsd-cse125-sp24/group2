#include "Packet.hpp"

union Union32 {
    float f;
    int i;
    uint32_t l;
} union32;
Packet::Packet() {}
Packet::~Packet() {}

void Packet::write_byte(char data) { buffer.push_back(data); }

int Packet::read_byte(char* dst) {
    if (buffer.size() < sizeof(char)) {
        return -1;
    }
    *dst = buffer.front();
    buffer.pop_front();
    return sizeof(char);
}

int Packet::read_int(int* dst) {
    if (buffer.size() < sizeof(int)) {
        return -1;
    }

    uint32_t tmp = 0;
    tmp |= buffer.front() << 24;
    buffer.pop_front();
    tmp |= buffer.front() << 16;
    buffer.pop_front();
    tmp |= buffer.front() << 8;
    buffer.pop_front();
    tmp |= buffer.front();
    buffer.pop_front();

    *dst = (int)ntohl(tmp);
    return sizeof(int);
}

void Packet::write(uint8_t* data, int len) {
    int i = 0;
    while (i < len) {
        buffer.push_back(data[i]);
        i++;
    }
}

void Packet::write_int(int data) {
    uint32_t tmp = htonl(data);
    buffer.push_back((tmp >> 24) & 0xFF);
    buffer.push_back((tmp >> 16) & 0xFF);
    buffer.push_back((tmp >> 8) & 0xFF);
    buffer.push_back((tmp) & 0xFF);
}

void Packet::write_vec3(glm::vec3 data) {
    union32.f = data.x;
    write_int(union32.l);
    union32.f = data.y;
    write_int(union32.l);
    union32.f = data.z;
    write_int(union32.l);
}

uint8_t* Packet::getBytes() { return &buffer[0]; }

int Packet::size() { return buffer.size(); }