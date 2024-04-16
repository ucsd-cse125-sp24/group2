#include "Server.hpp"

#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex _mutex;

int Server::teardown() { return 0; }

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
    while (1) {
        struct sockaddr_in client_sin;
        int addr_len = sizeof(client_sin);
        Socket* client_sock = psocket.accept((struct sockaddr*)&client_sin, (socklen_t*)&addr_len);
        if (client_sock->getFD() < 0) {
            perror("accept failed");
            return;
        }

        printf("Incoming connection from %s:%d\n", inet_ntoa(client_sin.sin_addr),
               client_sin.sin_port);
        int i = 0;
        std::unique_lock<std::mutex> lock(_mutex);
        for (; i < MAX_CLIENTS; i++) {
            // Find first free slot
            if (clients.find(i) != clients.end())
                continue;

            // TODO create new clients outside and only assign client ids as "slots" instead of creating new client objects
            Client* client = new Client(i, client_sock, client_sin);
            clients[i] = client;
            client->init();

            NetworkManager::instance().register_entity(&(*client->p));
            printf("Client (%s:%d) was assigned id %d. Server capacity: %d / %d\n",
                   inet_ntoa(client_sin.sin_addr), client_sin.sin_port, i, Server::clients.size(),
                   MAX_CLIENTS);

            std::thread(&Server::receive, this, client).detach();

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
        int read_bytes = client->clientsock->recv((char*)&buffer, expected_data_len, 0);
        if (read_bytes == 0) {       // Connection was closed
            std::lock_guard<std::mutex> lock(_mutex);
            clients.erase(client->id);
            std::cout << "Client " << client->id << " disconnected." << std::endl;
            delete client;
            return;
        } else if (read_bytes < 0) { // error
            std::lock_guard<std::mutex> lock(_mutex);
            clients.erase(client->id);
            std::cout << "recv failed" << std::endl;
            std::cout << "Client " << client->id << " disconnected." << std::endl;
            delete client;
            return;
        } else {
            printf("Received %d bytes from client %d\n", read_bytes, client->id);

            client->handle_packet(buffer);
        }

        memset(buffer, 0, 4096);
    }
}

int Server::send(int client_id, const char* data, const int data_len) {
    /*
    for (int i = 0; i < data_len; i++) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)(((uint8_t*)data)[i])
                  << " ";
    }
    std::cout << std::endl;
    */
    int sent_bytes = clients[client_id]->clientsock->send(data, data_len, 0);
    if (sent_bytes < 0) {
        printf("failed to send\n");
        return 1;
    }
    return 0;
}