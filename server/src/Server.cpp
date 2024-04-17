#include "Server.hpp"

#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex _mutex;
std::mutex handler_mutex;

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
            if (clients.find(i) != clients.end())
                continue;

            // TODO create new clients outside and only assign client ids as
            // "slots" instead of creating new client objects
            Client* client = new Client(i, client_sock, client_sin);
            clients[i] = client;
            client->init();

            NetworkManager::instance().register_entity(&(*client->p));
            printf(
                "Client (%s:%d) was assigned id %d. Server capacity: %d / %d\n",
                inet_ntoa(client_sin.sin_addr), client_sin.sin_port, i,
                Server::clients.size(), MAX_CLIENTS);

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
        int read_bytes =
            client->clientsock->recv((char*)&buffer, expected_data_len, 0);
        if (read_bytes == 0) { // Connection was closed
            std::lock_guard<std::mutex> lock(_mutex);
            clients.erase(client->id);
            std::cout << "Client " << client->id << " disconnected."
                      << std::endl;
            delete client;
            return;
        } else if (read_bytes < 0) { // error
            std::lock_guard<std::mutex> lock(_mutex);
            clients.erase(client->id);
            std::cout << "recv failed" << std::endl;
            std::cout << "Client " << client->id << " disconnected."
                      << std::endl;
            delete client;
            return;
        } else {
            printf("Received %d bytes from client %d\n", read_bytes,
                   client->id);

            std::lock_guard<std::mutex> lock(handler_mutex);
            if (receive_event) {
                // FIXME make a copy of buffer
                receive_event(buffer);
            }
        }

        memset(buffer, 0, 4096);
    }
}

void Server::set_callback(const ReceiveHandler& handler) {
    std::lock_guard<std::mutex> lock(handler_mutex);
    receive_event = handler;
}

int Server::send(int client_id, const char* data, const int data_len) {
    int sent_bytes = clients[client_id]->clientsock->send(data, data_len, 0);
    if (sent_bytes < 0) {
        printf("failed to send\n");
        return 1;
    }
    return 0;
}