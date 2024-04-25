#include "packet.hpp"

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
    u_int32_t l;
} union32;

union Union64 {
    double f;
    u_int64_t l;
} union64;

void Packet::write_byte(char data){
    u_int8_t toAdd = u_int8_t(data);
    buffer.push_back(toAdd);
}

// 32 bits
// ref: https://stackoverflow.com/questions/6499183/converting-a-uint32-value-into-a-uint8-array4
void Packet::write_int(int data){
    union32.i = data;
    u_int32_t bytes = union32.l;
    bytes = htonl(bytes);

    /* Equivalent code:
    u_int8_t byte_array[4];
    byte_array[0] = bytes >> 24;
    byte_array[1] = bytes >> 16;
    byte_array[2] = bytes >>  8;
    byte_array[3] = bytes;


    buffer.push_back(byte_array[0]);
    buffer.push_back(byte_array[1]);
    buffer.push_back(byte_array[2]);
    buffer.push_back(byte_array[3]);*/

    // store 4 bytes in the buffer
    for(int i = 1; i <= 4; i++){
        buffer.push_back((u_int8_t)(bytes >> 8*(4-i)));
    }

}

// 32 bits
void Packet::write_float(float data){
    union32.f = data;
    u_int32_t bytes = union32.l;
    bytes = htonl(bytes);

    // store 4 bytes in the buffer
    for(int i = 1; i <= 4; i++){
        buffer.push_back((u_int8_t)(bytes >> 8*(4-i)));
    }

}

// ref: https://stackoverflow.com/questions/809902/64-bit-ntohl-in-c
// ntoh function for 64 bit input
u_int64_t
ntoh64(const u_int64_t *input)
{
    u_int64_t rval;
    u_int8_t *data = (u_int8_t *)&rval;

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
void Packet::write_double(double data){
    union64.f = data;
    u_int64_t bytes = union64.l;

    //std::cout << "double is: " << data << " u_int64 is: " << bytes;

    bytes = ntoh64(&bytes);

    //std::cout << " sending: " << bytes << std::endl;

    // store 8 bytes in the buffer
    for(int i = 1; i <= 8; i++){
        u_int8_t toWrite = (u_int8_t)(bytes >> 8*(8-i));
        buffer.push_back(toWrite);
    }
}

// floats are 32 bits
// store x, y, z
void Packet::write_vec3(glm::vec3 data){
    // write each element of the data vector
    write_float(data.x);
    write_float(data.y);
    write_float(data.z);
}

int Packet::read_byte(char* dest){
    if(buffer.size() == 0){
        return -1;
    }

    u_int8_t readElem = buffer.front();
    buffer.pop_front();
    *dest = char(readElem);
    
    return sizeof(char);
}

int Packet::read_int(int* dest){
    int byte_per_int = sizeof(int);

    if(buffer.size() < byte_per_int){
        return -1;
    }

    // read 4 bytes from the buffer
    u_int32_t readData[byte_per_int];
    for(int i = 0; i < byte_per_int; i++){
        readData[i] = buffer.front();
        buffer.pop_front();
    }

    // reconstruct u_int32
    u_int32_t i32 = readData[3] | (readData[2] << 8) | (readData[1] << 16) | (readData[0] << 24);
    i32 = ntohl(i32);

    *dest = static_cast<int>(i32); // convert to int

    return byte_per_int;
}

int Packet::read_float(float* dest){
    int byte_per_float = sizeof(float);

    if(buffer.size() < byte_per_float){
        return -1;
    }

    // read 4 bytes from the buffer
    u_int32_t readData[byte_per_float];
    for(int i = 0; i < byte_per_float; i++){
        readData[i] = buffer.front();
        buffer.pop_front();
    }

    // reconstruct u_int32
    u_int32_t i32 = readData[3] | (readData[2] << 8) | (readData[1] << 16) | (readData[0] << 24);
    i32 = ntohl(i32);

    // convert to float
    union32.l = i32;
    *dest = (float)union32.f;

    return byte_per_float;
}

// credit: https://codereview.stackexchange.com/questions/2607/combining-two-32-bit-integers-into-one-64-bit-integer
int Packet::read_double(double* dest){ // use unsigned long long
    int byte_per_double = sizeof(double);

    if(buffer.size() < byte_per_double){
        return -1;
    }

    // read 8 bytes from the buffer
    u_int64_t readData[byte_per_double]; // u_int64 instead of u_int8 so shifts don't overflow
    for(int i = 0; i < byte_per_double; i++){
        readData[i] = buffer.front();
        buffer.pop_front();
    }

    // reconstruct u_int64
    u_int64_t i64 = readData[7] | (readData[6] << 8) | (readData[5] << 16) | (readData[4] << 24) | (readData[3] << 32) | (readData[2] << 40) | (readData[1] << 48) |  (readData[0] << 56);

    // convert to double
    i64 = ntoh64(&i64);
    union64.l = i64;
    *dest = union64.f;

    return byte_per_double;
}

int Packet::read_vec3(glm::vec3* dest){
    float x, y, z;
    // read each element of the vector, error checking size based on return val.
    if(read_float(&x) == -1){
        return -1;
    }
    if(read_float(&y) == -1){
        return -1;
    }
    if(read_float(&z) == -1){
        return -1;
    }

    // reconstruct vector
    *dest = glm::vec3(x, y, z);

    return sizeof(float)*3;
}

u_int8_t* Packet::getBytes() {
    uint8_t* byte_array = new uint8_t[buffer.size()];
    memcpy(byte_array, &buffer[0], buffer.size());

    return byte_array;
}