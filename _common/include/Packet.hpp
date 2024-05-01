#ifndef PACKET_H
#define PACKET_H
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <string.h>
#include <deque>

#ifdef _WIN32
/**
 *   To compile in MinGW use -lws2_32 linker option
 *   To compile with MSVC++ use  #pragma comment(lib,"Ws2_32.lib")
 **/
#include <winsock2.h>
#endif
enum class PacketType { PLAYER_POSITION, PLAYER_INPUT, STATE_UPDATE };
class Packet {

public:
    std::deque<uint8_t> buffer;

    void write(uint8_t*, int);
    void write_byte(char data);
    void write_int(int data);
    void write_float(float data);
    void write_double(double data);
    void write_vec3(glm::vec3 data);
    void write_quat(glm::quat data);

    // return -1 if error, else return size
    int read_byte(char* dest);
    int read_int(int* dest);
    int read_float(float* dest);
    int read_double(double* dest);
    int read_vec3(glm::vec3* dest);
    int read_quat(glm::quat* data);

    uint8_t* getBytes();
    int size();
};

#endif // PACKET_H