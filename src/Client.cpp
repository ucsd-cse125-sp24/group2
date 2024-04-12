#include "Client.h"
#include <stdio.h>
#include <thread>
#include <iostream>
#include <iomanip>
#include <glm/gtx/string_cast.hpp>

union FloatUnion {
    float f;
    uint32_t l;
} num;
void Client::init(Mover* m) {
    int res;
    res = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (res != 0) {
        printf("WSAStartup failed\n");
        return;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("failed to create socket\n");
        return;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_port = htons(25565);

    if (connect(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        perror("connection failed");
    }

    std::thread(receive, sock, m).detach();
}

void Client::receive(SOCKET sock, Mover* m) {
    uint8_t buf[4096];
    int read_bytes;
    do {
        read_bytes = recv(sock, (char*) &buf, 4096, 0);
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
            m->position = pos;

            // TODO process data
        } else if (read_bytes < 0) {
            printf("error in receive\n");
        } else {
            printf("disconnected\n");
        }
    }
    while (read_bytes > 0);

    closesocket(sock);
}

void Client::send(const char* buf, int data_len) {
    int sent_bytes = ::send(sock, buf, data_len, 0);
    if (sent_bytes < 0) {
        printf("failed to send\n");
    }
}