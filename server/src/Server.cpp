#include "Server.hpp"

#include <iostream>
#include <iomanip>

std::map<int, Client*> Server::clients;
Socket Server::psocket;
int Server::teardown() {
    return 0;
}

int Server::init() {
    int sock = psocket.socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);

    if (!psocket.bind((struct sockaddr*)&sin, sizeof(sin))) {
        perror("bind failed");
        return 1;
    }

    if (!psocket.listen(MAX_CLIENTS)) {
        perror("listen failed");
        return 1;
    }

    printf("Now listening...\n");
    while (1) {
        struct sockaddr_in client_sin;
        int addr_len = sizeof(client_sin);
        Socket* client_sock = psocket.accept((struct sockaddr*)&client_sin, (socklen_t*)&addr_len);
        if (client_sock->getFD() < 0) {
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
            // int res = pthread_create(&thread, NULL, Server::receive, client);

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
        int read_bytes = client->clientsock->recv((char*)&buffer, expected_data_len, 0);
        if (read_bytes == 0) {  // Connection was closed
            return;
        } else if (read_bytes < 0) {  // error
            psocket.close();
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
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)(((uint8_t*)data)[i]) << " ";
    }
    std::cout << std::endl;
    int sent_bytes = clients[client_id]->clientsock->send(data, data_len, 0);
    if (sent_bytes < 0) {
        printf("failed to send\n");
        return 1;
    }
    return 0;
}