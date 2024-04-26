#include "Server.hpp"

#include <iomanip>
#include <iostream>
#include <thread>

int Server::teardown() { return 0; }

// FIXME rare chance that server crashes silently on client disconnect
// likely due to segfault
void Server::start() {
    int sock = psocket.socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("socket() failed");
        return;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);

    if (!psocket.bind((struct sockaddr*)&sin, sizeof(sin))) {
        perror("bind failed");
        return;
    }

    if (!psocket.listen(MAX_CLIENTS)) {
        perror("listen failed");
        return;
    }
    printf("Now listening on port %d\n", SERVER_PORT);

    // Populate clients
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i] = new Client(i);
    }

    while (1) {
        struct sockaddr_in client_sin;
        int addr_len = sizeof(client_sin);
        Socket* client_sock = psocket.accept((struct sockaddr*)&client_sin,
                                             (socklen_t*)&addr_len);
        if (client_sock->getFD() < 0) {
            perror("accept failed");
            return;
        }

        printf("Incoming connection from %s:%d\n",
               inet_ntoa(client_sin.sin_addr), client_sin.sin_port);
        int i = 0;
        std::unique_lock<std::mutex> lock(_mutex);
        for (; i < MAX_CLIENTS; i++) {
            // Find first free slot
            if (clients[i]->clientsock != nullptr)
                continue;

            // FIXME move player initialization and registration out
            clients[i]->clientsock = client_sock;
            clients[i]->init();

            on_client_joined(i);

            // NetworkManager::instance().register_entity(clients[i]->p);
            // TODO track server capacity in a not-dumb way
            printf(
                "Client (%s:%d) was assigned id %d. Server capacity: %d / %d\n",
                inet_ntoa(client_sin.sin_addr), client_sin.sin_port, i,
                Server::clients.size(), MAX_CLIENTS);

            std::thread(&Server::receive, this, clients[i]).detach();

            break;
        }
        lock.unlock();

        if (i == MAX_CLIENTS) {
            perror("Server is full");
        }
    }
}

std::vector<Client*>* Server::get_clients() {
    std::vector<Client*>* res = new std::vector<Client*>();
    std::lock_guard<std::mutex> lock(_mutex);
    for (auto it : clients) {
        res->push_back(it.second);
    }
    return res;
}

// multi-threaded
void Server::receive(Client* client) {
    uint8_t buffer[4096];
    int expected_data_len = sizeof(buffer);

    while (1) {
        int read_bytes =
            client->clientsock->recv((char*)buffer, expected_data_len, 0);
        if (read_bytes == 0) { // Connection was closed
            std::lock_guard<std::mutex> lock(_mutex);
            // TODO have network manager handle disconnect events
            // NetworkManager::instance().unregister_entity(client->p);
            client->disconnect();
            std::cout << "Client " << client->id << " disconnected."
                      << std::endl;
            return;
        } else if (read_bytes < 0) { // error
            std::lock_guard<std::mutex> lock(_mutex);
            // NetworkManager::instance().unregister_entity(client->p);
            client->disconnect();
            std::cout << "recv failed" << std::endl;
            std::cout << "Client " << client->id << " disconnected."
                      << std::endl;
            return;
        } else {
            printf("Received %d bytes from client %d\n", read_bytes,
                   client->id);
            uint8_t* recvd_bytes = new uint8_t[read_bytes];
            memcpy(recvd_bytes, buffer, read_bytes);
            on_message_received(client->id, recvd_bytes);
        }

        memset(buffer, 0, 4096);
    }
}

int Server::send(int client_id, Packet* pkt) {
    // FIXME might not be thread-safe?
    if (clients[client_id]->clientsock == nullptr) {
        printf("failed to send\n");
        return 1;
    }

    int sent_bytes =
        clients[client_id]->clientsock->send(pkt->getBytes(), pkt->size(), 0);
    delete pkt;
    if (sent_bytes < 0) {
        printf("failed to send\n");
        return 1;
    }
    return 0;
}