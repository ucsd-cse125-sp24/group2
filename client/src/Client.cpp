#include "Client.h"
#include <glm/gtx/string_cast.hpp>
#include <iomanip>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include "GameManager.hpp"

void Client::connect(const char* ip, uint16_t port) {
    int sock = psocket.socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("[CLIENT] failed to create socket\n");
        return;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip);
    sin.sin_port = htons(port);

    // connect to server
    if (!psocket.connect((struct sockaddr*)&sin, sizeof(sin))) {
        perror("[CLIENT] connection failed");
        return;
    }
    printf("[CLIENT] successfully connected to %s:%d\n", ip, port);

    std::thread(&Client::receive, this).detach();
}

void Client::receive() {

    while (true) {
        int pktSize;
        int read_bytes = psocket.recv((char*)&pktSize, sizeof(pktSize), 0);

        if (read_bytes <= 0) {
            printf("[CLIENT] error in receive\n");
            break;
        }

        pktSize = ntohl(pktSize);
        int reversed; // 4 bytes reversed of getal
        uint8_t *n1, *n2;
        n1 = (uint8_t*)&pktSize;
        n2 = (uint8_t*)&reversed;

        n2[0] = n1[3];
        n2[1] = n1[2];
        n2[2] = n1[1];
        n2[3] = n1[0];

        pktSize = reversed;
        uint8_t* buf = new uint8_t[4096];

        int totalBytesRead = 0;
        while (totalBytesRead < pktSize) {
            read_bytes = psocket.recv((char*)(buf + totalBytesRead),
                                      pktSize - totalBytesRead, 0);

            if (read_bytes < 0) {
                printf("[CLIENT] error in receive\n");
                delete[] buf;
                return;
            } else if (read_bytes == 0) {
                printf("[CLIENT] disconnected\n");
                delete[] buf;
                return;
            }

            totalBytesRead += read_bytes;
        }

        Packet* pkt = new Packet();
        pkt->write((uint8_t*)buf, totalBytesRead);
        delete[] buf;

        std::lock_guard<std::mutex> lock(mutex);
        if (receive_event) {
            receive_event(pkt);
        }
    }
    /*
    do {
        if (read_bytes > 0) {
            printf("[CLIENT] received %d bytes from server\n", read_bytes);
            for (int i = 0; i < read_bytes; i++) {
                std::cout << std::setfill('0') << std::setw(2) << std::hex
                          << (int)buf[i] << " ";
            }
            std::cout << std::endl;

            Packet* pkt = new Packet();
            pkt->write((uint8_t*)buf, read_bytes);
            int pktsize;
            pkt->read_int(&pktsize);
            printf("packet size: %d\n", pktsize);

            std::lock_guard<std::mutex> lock(mutex);
            if (receive_event) {
                receive_event(pkt);
            }

        } else if (read_bytes < 0) {
            printf("[CLIENT] error in receive\n");
        } else {
            printf("[CLIENT] disconnected\n");
        }
    } while (read_bytes > 0);
    */
}

void Client::send(Packet* pkt) {
    int sent_bytes =
        psocket.send((const char*)(pkt->getBytes()), pkt->size(), 0);
    if (sent_bytes < 0) {
        printf("[CLIENT] failed to send\n");
    }
}

void Client::setCallback(const ReceiveHandler& callback) {
    std::lock_guard<std::mutex> lock(mutex);
    receive_event = callback;
}