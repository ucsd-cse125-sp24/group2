#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <map>
#include <string>
#include <mutex>
#include <functional>

#include "Client.hpp"
#include "NetworkManager.hpp"
#include "Event.hpp"
#include "psocket.hpp"
#include "EventArgs.hpp"

#define MAX_CLIENTS 1
#define SERVER_PORT 25565
class Server {
private:
    std::mutex _mutex;
    Socket psocket;
    void receive(Client* client);
    EventHandler<MessageReceivedEventArgs> message_received;
    EventHandler<ClientEventArgs> client_joined;
    EventHandler<ClientEventArgs> client_disconnected;

public:
    std::map<int, Client*> clients;

    void start();
    int teardown();
    int send(int, Packet*);
    std::map<int, Client*> get_clients();
    inline void set_message_received_callback(const Action& handler) {
        message_received += handler;
    }
    inline void set_client_joined_callback(const Action& handler) {
        client_joined += handler;
    }
    inline void set_client_disconnected_callback(const Action& handler) {
        client_disconnected += handler;
    }
}; // SERVER_H

#endif // SERVER_H