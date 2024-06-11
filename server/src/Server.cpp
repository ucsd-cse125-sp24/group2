#include "Server.hpp"

#include <iomanip>
#include <iostream>
#include <thread>
#include "ColorCodes.hpp"

size_t getThread() {
    return std::hash<std::thread::id> {}(std::this_thread::get_id());
}

int Server::teardown() { return 0; }

// FIXME rare chance that server crashes silently on client disconnect
// likely due to segfault
void Server::start() {
    int sock = psocket.socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("[SERVER] socket() failed");
        return;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);

    if (!psocket.bind((struct sockaddr*)&sin, sizeof(sin))) {
        perror("[SERVER] bind failed");
        return;
    }

    if (!psocket.listen(MAX_CLIENTS)) {
        perror("listen failed");
        return;
    }
    printf("[SERVER] Now listening on port %d\n", SERVER_PORT);

    while (1) {
        struct sockaddr_in client_sin;
        int addr_len = sizeof(client_sin);
        Socket* client_sock = psocket.accept((struct sockaddr*)&client_sin,
                                             (socklen_t*)&addr_len);
        if (client_sock->getFD() < 0) {
            perror("accept failed");
            return;
        }

        printf("[SERVER] Incoming connection from %s:%d\n",
               inet_ntoa(client_sin.sin_addr), client_sin.sin_port);
        std::unique_lock<std::mutex> lock(_mutex);
        int newClientId = -1;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            // Find first free slot
            if (clients.find(i) != clients.end())
                continue;

            newClientId = i;
            clients[newClientId] = new Client(newClientId);
            clients[newClientId]->clientsock = client_sock;

            printf("[SERVER] Client (%s:%d) was assigned id %d. Server "
                   "capacity: %d / %d\n",
                   inet_ntoa(client_sin.sin_addr), client_sin.sin_port,
                   newClientId, Server::clients.size(), MAX_CLIENTS);

            std::thread(&Server::receive, this, clients[newClientId]).detach();

            break;
        }
        lock.unlock();

        if (newClientId == -1) {
            printf(RED "[SERVER] Server is full\n" RST);
            continue;
        }

        // Raise client joined event
        auto args = new ClientEventArgs(newClientId);
        client_joined.invoke(args);
    }
}

std::map<int, Client*> Server::get_clients() {
    std::lock_guard<std::mutex> lock(_mutex);
    std::map<int, Client*> res(clients);
    return res;
}

// new thread per client
void Server::receive(Client* client) {
    while (1) {
        int pktSize;
        int read_bytes =
            client->clientsock->recv((char*)&pktSize, sizeof(pktSize), 0);

        if (read_bytes <= 0) {
            printf("[SERVER] error in receive\n");
            break;
        }

        pktSize = ntohl(pktSize);
        int reversed;
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
        int clientId = client->id;
        while (totalBytesRead < pktSize) {
            read_bytes = client->clientsock->recv((char*)(buf + totalBytesRead),
                                                  pktSize - totalBytesRead, 0);

            if (read_bytes < 0) {
                printf("[SERVER] error in receive\n");
                delete[] buf;
                return;
            } else if (read_bytes == 0) {
                auto args = new ClientEventArgs(clientId);
                client_disconnected.invoke(args);
                std::cout << "[SERVER " << getThread() << "] Client "
                          << clientId << " disconnected." << std::endl;
                std::lock_guard<std::mutex> lock(_mutex);
                delete client;
                clients.erase(clientId);
                delete[] buf;
                return;
            }

            totalBytesRead += read_bytes;
        }

        Packet* pkt = new Packet();
        pkt->write((uint8_t*)buf, totalBytesRead);

        MessageReceivedEventArgs* args =
            new MessageReceivedEventArgs(client->id, buf, read_bytes);
        message_received.invoke(args);
    }

    /*
    uint8_t buffer[4096];
    int expected_data_len = sizeof(buffer);

    int clientId = client->id;
    while (1) {
        int read_bytes =
            client->clientsock->recv((char*)buffer, expected_data_len, 0);
        // TODO handle disconnect events
        if (read_bytes == 0) { // Connection was closed
            std::lock_guard<std::mutex> lock(_mutex);
            client->disconnect();
            break;
        } else if (read_bytes < 0) { // error
            std::lock_guard<std::mutex> lock(_mutex);
            client->disconnect();
            break;
        } else {
            // TODO handle multiple packets per receive call
            printf("[SERVER %lu] Received %d bytes from client %d\n",
                   getThread(), read_bytes, client->id);
            uint8_t* recvd_bytes = new uint8_t[read_bytes];
            memcpy(recvd_bytes, buffer, read_bytes);

            MessageReceivedEventArgs* args =
                new MessageReceivedEventArgs(client->id, buffer, read_bytes);
            message_received.invoke(args);
        }

        memset(buffer, 0, 4096);
    }

    auto args = new ClientEventArgs(clientId);
    client_disconnected.invoke(args);
    std::cout << "[SERVER " << getThread() << "] Client " << clientId
              << " disconnected." << std::endl;
    std::lock_guard<std::mutex> lock(_mutex);
    delete client;
    clients.erase(clientId);
    */
}

int Server::send(int client_id, Packet* pkt) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (clients.find(client_id) == clients.end()) {
        printf("[SERVER] failed to send - client disconnected\n");
        return 1;
    }

    if (clients[client_id]->clientsock == nullptr) {
        printf("[SERVER] failed to send - bad socket fd\n");
        return 1;
    }

    Packet* newPkt = new Packet();
    newPkt->write_int(pkt->size());
    newPkt->write(pkt->getBytes(), pkt->size());

    int sent_bytes = clients[client_id]->clientsock->send(
        (const char*)newPkt->getBytes(), newPkt->size(), 0);

    delete pkt;
    delete newPkt;
    if (sent_bytes < 0) {
        printf("[SERVER] failed to send\n");
        return -1;
    }
    return 0;
}