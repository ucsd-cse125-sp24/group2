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

    pthread_t thread;
    int res = pthread_create(&thread, NULL, Client::receive, this);

    GameManager::instance().object_destroyed += [this](EventArgs* e) {
        DestroyedEventArgs* args = (DestroyedEventArgs*)e;

        Packet* destroyed_ack = new Packet();
        destroyed_ack->write_int((int) PacketType::DESTROY_OBJECT_ACK);
        destroyed_ack->write_int(args->destroyedObjectIds.size());
        for (int destroyedObjId : args->destroyedObjectIds) {
            destroyed_ack->write_int(destroyedObjId);
        }
        send(destroyed_ack);
    };
}

void* Client::receive(void* params) {
    Client* client = (Client*)params;
    uint8_t buf[4096];
    int read_bytes;
    do {
        read_bytes = client->psocket.recv((char*)buf, 4096, 0);
        if (read_bytes > 0) {
            for (int i = 0; i < read_bytes; i++) {
                std::cout << std::setfill('0') << std::setw(2) << std::hex
                          << (int)buf[i] << " ";
            }
            std::cout << std::endl;
            printf("[CLIENT] received %d bytes from server\n", read_bytes);

            Packet* pkt = new Packet();
            pkt->write((uint8_t*)buf, read_bytes);

            std::lock_guard<std::mutex> lock(client->mutex);
            if (client->receive_event) {
                client->receive_event(pkt);
            }

        } else if (read_bytes < 0) {
            printf("[CLIENT] error in receive\n");
        } else {
            printf("[CLIENT] disconnected\n");
        }
    } while (read_bytes > 0);
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