#include "packet.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
/**
 * Run these tests:
 * cd to group2/tests/packet directory
 * 'make'
 * './Packet'
*/

using namespace std;

/**
 * Simple tests: write and read back byte, double, float, int, and glm::vec3
*/
void basic_tests(Packet packet){
    char c = 'a';
    double d = 3.456;
    float f = 6.45;
    int i = 2;
    glm::vec3 v = glm::vec3(1.2, 7.8, 0.9);
    glm::quat q = glm::quat(8.1, 0.3, 9.7, 5.4);

    cout << "---------- Starting Basic Packet Tests ----------" << endl;
    packet.write_byte(c);
    cout << "write byte: " << c << endl;
    packet.write_double(d);
    cout << "write double: " << d << endl;
    packet.write_float(f);
    cout << "write float: " << f << endl;
    packet.write_int(i);
    cout << "write int: " << i << endl;
    packet.write_vec3(v);
    cout << "write vector: (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
    packet.write_quat(q);
    cout << "write quaternion: (" << q.w << ", " << q.x  << ", " << q.y << ", " << q.z << ")" << endl;

    // reset variables (avoid false passes)
    c = '1';
    d = 0.1;
    f = 0.01;
    i = 1;
    v = glm::vec3(0.0, 0.1, 0.0);
    q = glm::quat(0.1, 0.0, 0.0, 1.0);
    cout << endl;

    packet.read_byte(&c);
    cout << "read char: " << c << endl;
    packet.read_double(&d);
    cout << "read double: " << d << endl;
    packet.read_float(&f);
    cout << "read float: " << f << endl;
    packet.read_int(&i);
    cout << "read int: " << i << endl;
    packet.read_vec3(&v);
    cout << "read vector: (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
    packet.read_quat(&q);
    cout << "read quaternion: (" << q.w << ", " << q.x  << ", " << q.y << ", " << q.z << ")" << endl;
}

/**
 * Test error handling on size mismatch
 * 1. Attempting read beyond capacity of the buffer (reading 64b from 32b data)
 * 2. Attempting read on empty buffer
*/
void size_mismatch_tests(Packet packet){
    double d;
    float f1 = 1.23456789;
    float f2, f3;

    cout << "---------- Starting Read Mismatch Packet Tests ----------" << endl;
    packet.write_float(f1);
    cout << "write float: " << f1 << endl;

    cout << endl;

    cout << "attempting to read a double..." << endl;
    cout << "read returns " << packet.read_double(&d);
    cout << " with result: " << d << endl;
    cout << endl;

    cout << "attempting to read a float..." << endl;
    cout << "read returns " << packet.read_float(&f2);
    cout << " with result: " << f2 << endl;
    cout << endl;

    cout << "attempting to read a float..." << endl;
    cout << "read returns " << packet.read_float(&f3);
    cout << " with result: " << f3 << endl;
}

int main(int argc, char** argv) {
    Packet packet = Packet();
    basic_tests(packet);
    cout << endl;
    size_mismatch_tests(packet);
}