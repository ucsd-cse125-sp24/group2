#include "Client.h"
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <iomanip>
#include <glm/gtx/string_cast.hpp>

union FloatUnion {
    float f;
    uint32_t l;
} num;
void Client::init(Mover* m) {
    this->m = m;
    int sock = psocket.socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("failed to create socket\n");
        return;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_port = htons(25565);

    if (!psocket.connect((struct sockaddr*)&sin, sizeof(sin))) {
        perror("connection failed");
        return;
    }

    pthread_t thread;
    int res = pthread_create(&thread, NULL, Client::receive, this);
}

void* Client::receive(void* params) {
    Client* client = (Client*)params;
    char buf[4096];
    int read_bytes;
    do {
        read_bytes = client->psocket.recv(buf, 4096, 0);
        if (read_bytes > 0) {
            for (int i = 0; i < read_bytes; i++) {
                std::cout << std::setfill('0') << std::setw(2) << std::hex << (int)buf[i] << " ";
            }
            std::cout << std::endl;
            printf("received %d bytes from server\n", read_bytes);

            uint32_t tmp;
            memcpy(&tmp, buf, 4);
            num.l = ntohl(tmp);
            float x = num.f;

            memcpy(&tmp, buf + 4, 4);
            num.l = ntohl(tmp);
            float y = num.f;

            memcpy(&tmp, buf + 8, 4);
            num.l = ntohl(tmp);
            float z = num.f;

            glm::vec3 pos = glm::vec3(x, y, z);
            client->m->position = pos;

            // TODO process data
        } else if (read_bytes < 0) {
            printf("error in receive\n");
        } else {
            printf("disconnected\n");
        }
    } while (read_bytes > 0);
}

void Client::send(const char* buf, int data_len) {
    int sent_bytes = psocket.send(buf, data_len, 0);
    if (sent_bytes < 0) {
        printf("failed to send\n");
    }
}