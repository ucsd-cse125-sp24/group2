#include "packet.hpp"

// TODO: read from beginning and error check for buffer size

union Union32 {
    float f;
    int i;
    u_int32_t l;
} union32;

union Union64 {
    double f;
    u_int64_t l;
} union64;

Packet::Packet() {
    this->buffer = std::vector<u_int8_t>();
}

Packet::~Packet() {
    this->buffer = std::vector<u_int8_t>();
}

// TODO: union for this?
void Packet::write_byte(char data){
    u_int8_t toAdd = u_int8_t(data);
    this->buffer.push_back(toAdd);
}

// 32 bits
// ref: https://stackoverflow.com/questions/6499183/converting-a-uint32-value-into-a-uint8-array4
void Packet::write_int(int data){
    union32.i = data;
    u_int32_t bytes = union32.l;
    bytes = htonl(bytes);

    /*u_int8_t byte_array[4];
    byte_array[0] = bytes >> 24;
    byte_array[1] = bytes >> 16;
    byte_array[2] = bytes >>  8;
    byte_array[3] = bytes;


    this->buffer.push_back(byte_array[0]);
    this->buffer.push_back(byte_array[1]);
    this->buffer.push_back(byte_array[2]);
    this->buffer.push_back(byte_array[3]);*/

    for(int i = 1; i <= 4; i++){
        //std::cout << "shift: " << 8*(4-i) << " bytes is: " << bytes << std::endl;
        this->buffer.push_back((u_int8_t)(bytes >> 8*(4-i)));
    }

}

// 32 bits
void Packet::write_float(float data){
    union32.f = data;
    u_int32_t bytes = union32.l;
    bytes = htonl(bytes);

    for(int i = 1; i <= 4; i++){
        //std::cout << "shift: " << 8*(4-i) << " bytes is: " << bytes << std::endl;
        this->buffer.push_back((u_int8_t)(bytes >> 8*(4-i)));
    }

}

uint64_t
ntoh64(const uint64_t *input)
{
    uint64_t rval;
    uint8_t *data = (uint8_t *)&rval;

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

    u_int64_t temp = 0;
    for(int i = 1; i <= 8; i++){
        //std::cout << "shift: " << 8*(8-i);
        u_int8_t toWrite = (u_int8_t)(bytes >> 8*(8-i));
        //std::cout << " writing: " << (int)toWrite << std::endl;
        //u_int64_t intermediate = toWrite;
        //temp = temp | (intermediate << 8*(8-i));
        this->buffer.push_back(toWrite);
    }
    //std::cout << "reconstructed: " << temp << std::endl;
}

// floats are 32 bits
// store x, y, z
void Packet::write_vec3(glm::vec3 data){
    write_float(data.x);
    write_float(data.y);
    write_float(data.z);
}

int Packet::read_byte(char* dest){
    if(this->buffer.size() == 0){
        return -1;
    }

    u_int8_t readElem = this->buffer.front();
    this->buffer.erase(this->buffer.begin());
    *dest = char(readElem);
    std::cout << "read char: " << *dest << std::endl;
    return 1;
}

int Packet::read_int(int* dest){
    int int8_per_int = 4;

    if(this->buffer.size() < int8_per_int){
        return -1;
    }

    u_int8_t readData[int8_per_int];
    for(int i = 0; i < int8_per_int; i++){
        readData[i] = this->buffer.front();
        this->buffer.erase(this->buffer.begin());
    }

    uint32_t i32 = readData[3] | (readData[2] << 8) | (readData[1] << 16) | (readData[0] << 24);
    i32 = htonl(i32);

    *dest = static_cast<int>(i32);

    std::cout << "read int: " << *dest << std::endl;
    return 32;
}

int Packet::read_float(float* dest){
    int int8_per_float = 4;

    if(this->buffer.size() < int8_per_float){
        return -1;
    }

    u_int8_t readData[int8_per_float];
    for(int i = 0; i < int8_per_float; i++){
        readData[i] = this->buffer.front();
        this->buffer.erase(this->buffer.begin());
    }

    uint32_t i32 = readData[3] | (readData[2] << 8) | (readData[1] << 16) | (readData[0] << 24);
    i32 = htonl(i32);
    union32.l = i32;
    *dest = (float)union32.f;

    std::cout << "read float: " << *dest << std::endl;
    return 32;
}

// credit: https://codereview.stackexchange.com/questions/2607/combining-two-32-bit-integers-into-one-64-bit-integer
int Packet::read_double(double* dest){ // use unsigned long long
// htono(long) -> type stays intact in the network -> little endian on network, but host could be either
    int int8_per_double = 8;

    if(this->buffer.size() < int8_per_double){
        return -1;
    }

    u_int64_t readData[int8_per_double]; // u_int64 so shifts don't overflow
    //u_int64_t temp = 0;
    for(int i = 0; i < int8_per_double; i++){
        readData[i] = this->buffer.front();
        //temp = temp | (this->buffer.front() << 8*(7-i));
        //std::cout << "read: " << (int)this->buffer.front() << " with shift: " << (8*(7-i)) << " result: " << temp << std::endl;
        this->buffer.erase(this->buffer.begin());
    }
    uint64_t i64 = readData[7] | (readData[6] << 8) | (readData[5] << 16) | (readData[4] << 24) | (readData[3] << 32) | (readData[2] << 40) | (readData[1] << 48) |  (readData[0] << 56);

    i64 = ntoh64(&i64);
    union64.l = i64;

    *dest = union64.f;
    // TODO: check that this is thread-safe?

    std::cout << "read double: " << *dest << std::endl;
    return int8_per_double*8;
}

int Packet::read_vec3(glm::vec3* dest){
    float x, y, z;
    if(read_float(&x) == -1){
        return -1;
    }
    if(read_float(&y) == -1){
        return -1;
    }
    if(read_float(&z) == -1){
        return -1;
    }
    std::cout << "read vector: " << x << ", " << y << ", " << z << std::endl;
    *dest = glm::vec3(x, y, z);
    return 32*3;
}



u_int8_t* Packet::getBytes() {
    u_int8_t* byte_array = (u_int8_t*)malloc(this->buffer.size() * sizeof(u_int8_t));
    int i = 0;

    for(u_int8_t b : this->buffer){
       byte_array[i++] = b;
    }

    return byte_array;

}