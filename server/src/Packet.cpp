#include "Packet.hpp"

Packet::Packet() {}

int Packet::read_byte(char* dst) {
    if (buffer.size() < sizeof(int)) {
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
    *dst = ntohl(buffer.front());
    buffer.pop_front();
    return sizeof(int);
}

void Packet::write(uint8_t* data, int len) {
    int i = 0;
    while (i < len) {
        buffer.push_back(*data);
        i++;
    }
}

void Packet::write_int(int data) { buffer.push_back(htonl(data)); }

uint8_t* Packet::getBytes() { return &buffer[0]; }

int Packet::size() { return buffer.size(); }