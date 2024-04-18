#define TICK_RATE_USEC 33333

#include "packet.hpp"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <chrono>
#include <sched.h>
#include <pthread.h>

using namespace std;
int main(int argc, char** argv) {
    Packet packet = Packet();
    packet.write_byte('a');
    packet.write_double(3.456);
    packet.write_int(2);
    packet.write_vec3(glm::vec3(1.2, 7.8, 0.9));
    glm::vec3 v = packet.read_vec3();
    int i = packet.read_int();
    double d = packet.read_double();
    char b = packet.read_byte();
}