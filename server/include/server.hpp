#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <map>
#include <string>
#include <mutex>
#include <functional>

#include "Client.hpp"
#include "NetworkManager.hpp"
#include "psocket.hpp"
#include "Packet.hpp"

#define MAX_CLIENTS 8
#define SERVER_PORT 25565

typedef std::function<void(void*)> ReceiveHandler;
class Server {
private:
    std::mutex _mutex;
    std::mutex handler_mutex;
    std::map<int, Client*> clients;
    Socket psocket;
    void receive(Client* client);
    ReceiveHandler receive_event = nullptr;

public:
    void start();
    int teardown();
    int send(int, Packet*);
    std::vector<Client*>* get_clients();
    void set_callback(const ReceiveHandler& handler);
}; // SERVER_H

#endif // SERVER_H