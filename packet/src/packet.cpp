#include "packet.hpp"

Packet::Packet() {
    this->buffer = std::vector<int32_t>();
    //this->buffer_position = 0;
}

Packet::~Packet() {
#ifdef _WIN32
    WSACleanup();
#endif
    this->buffer = std::vector<int32_t>();
}

void Packet::write_byte(char data){
    int32_t toAdd = int32_t(data);
    this->buffer.push_back(toAdd);
}

// 32 bits
void Packet::write_int(int data){
    this->buffer.push_back((int32_t)data);
}

// 64 bits
// upper then lower
void Packet::write_double(double data){
    int64_t data_i = static_cast<int64_t>(data);
    int32_t upper = static_cast<int32_t>(data_i >> 32);
    this->buffer.push_back(upper);
    int32_t lower = static_cast<int32_t>(data_i);
    this->buffer.push_back(lower);
}

// floats are 32 bits
// store x, y, z
void Packet::write_vec3(glm::vec3 data){
    this->buffer.push_back((int32_t)data.x);
    this->buffer.push_back((int32_t)data.y);
    this->buffer.push_back((int32_t)data.z);
}

char Packet::read_byte(){
    int32_t readElem = this->buffer.back();
    this->buffer.pop_back();
    char result = char(readElem);
    std::cout << result << std::endl;
    return result;
}

int Packet::read_int(){
    int result = (int)(this->buffer.back());
    this->buffer.pop_back();
    std::cout << result << std::endl;
    return result;
}

// credit: https://codereview.stackexchange.com/questions/2607/combining-two-32-bit-integers-into-one-64-bit-integer
double Packet::read_double(){
    int32_t lower = this->buffer.back();
    this->buffer.pop_back();
    int32_t upper = this->buffer.back();
    this->buffer.pop_back();
    int64_t data = (((int64_t) lower) << 32) | ((int64_t) upper);
    double result = static_cast<double>(data);
    std::cout << result << std::endl;
    return result;
}

glm::vec3 Packet::read_vec3(){
    int32_t z = this->buffer.back();
    this->buffer.pop_back();
    int32_t y = this->buffer.back();
    this->buffer.pop_back();
    int32_t x = this->buffer.back();
    this->buffer.pop_back();
    std::cout << x << ", " << y << ", " << z << std::endl;
    return glm::vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
}



int32_t* Packet::getBytes() {
    int32_t* byte_array = (int32_t*)malloc(this->buffer.size() * sizeof(int32_t));
    int i = 0;

    for(int32_t b : this->buffer){
       byte_array[i++] = b;
    }

    return nullptr;//byte_array;

}