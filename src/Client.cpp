#include "Client.h"
#include <stdio.h>
#include <thread>

void Client::init() {
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

    std::thread(receive, sock).detach();
}

void Client::receive(SOCKET sock) {
    char buf[4096];
    int read_bytes;
    do {
        read_bytes = recv(sock, buf, 4096, 0);
        if (read_bytes > 0) {
            printf("received %d bytes from server\n", read_bytes);
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