#include "Server.hpp"
#include <ws2tcpip.h>

#include <iostream>
#include  <iomanip>

std::map<int, Client*> Server::clients;
WSADATA Server::wsa_data;
int Server::teardown() {
    WSACleanup();
    return 0;
}

int Server::init() {
    int res;
    res = WSAStartup(MAKEWORD(2, 2), &Server::wsa_data);
    if (res != 0) {
        perror("WSAStartup failed");
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);

    if (bind(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        perror("bind failed");
        return 1;
    }

    if (listen(sock, MAX_CLIENTS) < 0) {
        perror("listen failed");
        return 1;
    }

    printf("Now listening...\n");
    while (1) {
        struct sockaddr_in client_sin;
        int addr_len = sizeof(client_sin);
        int client_sock = accept(sock, (struct sockaddr*)&client_sin, (socklen_t*)&addr_len);
        if (client_sock < 0) {
            perror("accept failed");
            return -1;
        }

        printf("Client (%s) connected on port %d\n", inet_ntoa(client_sin.sin_addr), client_sin.sin_port);
        int i = 0;
        for (i = 0; i < MAX_CLIENTS; i++) {
            // Find first free slot
            if (Server::clients.find(i) != Server::clients.end()) continue;

            Client* client = new Client(i, client_sock, client_sin);
            Server::clients[i] = client;
            client->init();

            NetworkManager::instance().register_entity(&(*client->p));
            printf("Client (%s:%d) was assigned id %d. Server capacity: %d / %d\n", inet_ntoa(client_sin.sin_addr), client_sin.sin_port, i, Server::clients.size(), MAX_CLIENTS);

            std::thread(Server::receive, client).detach();
            //int res = pthread_create(&thread, NULL, Server::receive, client);

            break;
        }

        if (i == MAX_CLIENTS) {
            perror("Server is full");
        }
    }

    return 0;
}

void Server::receive(Client* client) {
    uint8_t buffer[4096];
    int expected_data_len = sizeof(buffer);

    while (1) {
        int read_bytes = recv(client->sockfd, (char*)&buffer, expected_data_len, 0);
        if (read_bytes == 0) {  // Connection was closed
            return;
        } else if (read_bytes < 0) {  // error
            closesocket(client->sockfd);
            perror("recv failed");
            return;
        } else {
            printf("Received %d bytes from client %d\n", read_bytes, client->id);

            client->handle_packet(buffer);
        }

        memset(buffer, 0, 4096);
    }
}

int Server::send(int client_id, const char* data, const int data_len) {
    for (int i = 0; i < data_len; i++) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)(((uint8_t*) data)[i]) << " ";
    }
    std::cout << std::endl;
    int sent_bytes = ::send(Server::clients[client_id]->sockfd, data, data_len, 0);
    if (sent_bytes < 0) {
        printf("failed to send\n");
        return 1;
    }
    return 0;
}