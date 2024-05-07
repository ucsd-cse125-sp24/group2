#include "Packet.hpp"

#ifdef _WIN32
#include <winsock2.h>
#endif

/**
 * Packet class
 * Write and Read - byte, int, float, double, glm::vec3
 * Read returns -1 on error, size of data read (in bytes) otherwise
 *
 * Note: currently no support for checking if read in the wrong order
 *      or if read type mismatches from original written value
 */

union Union32 {
    float f;
    int i;
    uint32_t l;
} union32;

union Union64 {
    double f;
    uint64_t l;
} union64;

void Packet::write(uint8_t* data, int len) {
    int i = 0;
    while (i < len) {
        buffer.push_back(data[i]);
        i++;
    }
}

void Packet::write_byte(char data) { buffer.push_back(uint8_t(data)); }

// 32 bits
// ref:
// https://stackoverflow.com/questions/6499183/converting-a-uint32-value-into-a-uint8-array4
void Packet::write_int(int data) {
    union32.i = data;
    uint32_t bytes = union32.l;
    bytes = htonl(bytes);

    /* Equivalent code:
    uint8_t byte_array[4];
    byte_array[0] = bytes >> 24;
    byte_array[1] = bytes >> 16;
    byte_array[2] = bytes >>  8;
    byte_array[3] = bytes;


    buffer.push_back(byte_array[0]);
    buffer.push_back(byte_array[1]);
    buffer.push_back(byte_array[2]);
    buffer.push_back(byte_array[3]);*/

    // store 4 bytes in the buffer
    for (int i = 1; i <= 4; i++) {
        buffer.push_back((uint8_t)(bytes >> 8 * (4 - i)));
    }
}

// 32 bits
void Packet::write_float(float data) {
    union32.f = data;
    uint32_t bytes = union32.l;
    bytes = htonl(bytes);

    // store 4 bytes in the buffer
    for (int i = 1; i <= 4; i++) {
        buffer.push_back((uint8_t)(bytes >> 8 * (4 - i)));
    }
}

// ref: https://stackoverflow.com/questions/809902/64-bit-ntohl-in-c
// ntoh function for 64 bit input
uint64_t ntoh64(const uint64_t* input) {
    uint64_t rval;
    uint8_t* data = (uint8_t*)&rval;

    data[0] = *input >> 56;
    data[1] = *input >> 48;
    data[2] = *input >> 40;
    data[3] = *input >> 32;
    data[4] = *input >> 24;
    data[5] = *input >> 16;
    data[6] = *input >> 8;
    data[7] = *input >> 0;

    return rval;
}

// 64 bits
void Packet::write_double(double data) {
    union64.f = data;
    uint64_t bytes = union64.l;

    bytes = ntoh64(&bytes);

    // store 8 bytes in the buffer
    for (int i = 1; i <= 8; i++) {
        uint8_t toWrite = (uint8_t)(bytes >> 8 * (8 - i));
        buffer.push_back(toWrite);
    }
}

// floats are 32 bits
// store x, y, z
void Packet::write_vec3(glm::vec3 data) {
    // write each element of the data vector
    write_float(data.x);
    write_float(data.y);
    write_float(data.z);
}

// store w, x, y, z (constructor order)
void Packet::write_quat(glm::quat data) {
    // write each element of the data vector
    write_float(data.w);
    write_float(data.x);
    write_float(data.y);
    write_float(data.z);
}

int Packet::read_byte(char* dest) {
    if (buffer.size() == 0) {
        return -1;
    }

    uint8_t readElem = buffer.front();
    buffer.pop_front();
    *dest = char(readElem);

    return sizeof(char);
}

int Packet::read_int(int* dest) {
    int byte_per_int = sizeof(int);

    if (buffer.size() < byte_per_int) {
        return -1;
    }

    // read 4 bytes from the buffer
    uint32_t readData[byte_per_int];
    for (int i = 0; i < byte_per_int; i++) {
        readData[i] = buffer.front();
        buffer.pop_front();
    }

    // reconstruct uint32
    uint32_t i32 = readData[3] | (readData[2] << 8) | (readData[1] << 16) |
                   (readData[0] << 24);
    i32 = ntohl(i32);

    *dest = static_cast<int>(i32); // convert to int

    return byte_per_int;
}

int Packet::read_float(float* dest) {
    int byte_per_float = sizeof(float);

    if (buffer.size() < byte_per_float) {
        return -1;
    }

    // read 4 bytes from the buffer
    uint32_t readData[byte_per_float];
    for (int i = 0; i < byte_per_float; i++) {
        readData[i] = buffer.front();
        buffer.pop_front();
    }

    // reconstruct uint32
    uint32_t i32 = readData[3] | (readData[2] << 8) | (readData[1] << 16) |
                   (readData[0] << 24);
    i32 = ntohl(i32);

    // convert to float
    union32.l = i32;
    *dest = (float)union32.f;

    return byte_per_float;
}

// credit:
// https://codereview.stackexchange.com/questions/2607/combining-two-32-bit-integers-into-one-64-bit-integer
int Packet::read_double(double* dest) { // use unsigned long long
    int byte_per_double = sizeof(double);

    if (buffer.size() < byte_per_double) {
        return -1;
    }

    // read 8 bytes from the buffer
    uint64_t readData[byte_per_double]; // uint64 instead of uint8 so shifts
                                        // don't overflow
    for (int i = 0; i < byte_per_double; i++) {
        readData[i] = buffer.front();
        buffer.pop_front();
    }

    // reconstruct uint64
    uint64_t i64 = readData[7] | (readData[6] << 8) | (readData[5] << 16) |
                   (readData[4] << 24) | (readData[3] << 32) |
                   (readData[2] << 40) | (readData[1] << 48) |
                   (readData[0] << 56);

    // convert to double
    i64 = ntoh64(&i64);
    union64.l = i64;
    *dest = union64.f;

    return byte_per_double;
}

int Packet::read_vec3(glm::vec3* dest) {
    float x, y, z;
    // read each element of the vector, error checking size based on return val.
    if (read_float(&x) == -1) {
        return -1;
    }
    if (read_float(&y) == -1) {
        return -1;
    }
    if (read_float(&z) == -1) {
        return -1;
    }

    // reconstruct vector
    *dest = glm::vec3(x, y, z);

    return sizeof(float) * 3;
}

int Packet::read_quat(glm::quat* dest) {
    float w, x, y, z;
    // read each element of the vector, error checking size based on return val.
    if (read_float(&w) == -1) {
        return -1;
    }
    if (read_float(&x) == -1) {
        return -1;
    }
    if (read_float(&y) == -1) {
        return -1;
    }
    if (read_float(&z) == -1) {
        return -1;
    }

    // reconstruct quaternion
    *dest = glm::quat(w, x, y, z);

    return sizeof(float) * 4;
}

uint8_t* Packet::getBytes() {
    uint8_t* byte_array = new uint8_t[buffer.size()];
    memcpy(byte_array, &buffer[0], buffer.size());

    return byte_array;
}

int Packet::size() { return buffer.size(); }