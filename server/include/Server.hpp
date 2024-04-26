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
#include "EventArgs.hpp"

#define MAX_CLIENTS 8
#define SERVER_PORT 25565
typedef std::function<void(EventArgs*)> EventHandler;
class Server {
private:
    std::mutex _mutex;
    std::mutex message_received_mutex;
    std::mutex client_joined_mutex;
    Socket psocket;
    void receive(Client* client);
    EventHandler message_received = nullptr;
    EventHandler client_joined = nullptr;

public:
    std::map<int, Client*> clients;
    void start();
    int teardown();
    int send(int, Packet*);
    std::vector<Client*>* get_clients();
    inline void set_message_received_callback(const EventHandler& handler) {
        std::lock_guard<std::mutex> lock(message_received_mutex);
        message_received = handler;
    }
    inline void set_client_joined_callback(const EventHandler& handler) {
        std::lock_guard<std::mutex> lock(client_joined_mutex);
        client_joined = handler;
    }
    inline void on_message_received(int clientId, uint8_t* buffer) {
        std::lock_guard<std::mutex> lock(message_received_mutex);
        if (message_received) {
            MessageReceivedEventArgs* args = new MessageReceivedEventArgs(clientId, buffer);
            message_received(args);
        }
    }
    inline void on_client_joined(int clientId) {
        std::lock_guard<std::mutex> lock(client_joined_mutex);
        if (client_joined) {
            ClientJoinedEventArgs* args = new ClientJoinedEventArgs(clientId);
            client_joined(args);
        }
    }
}; // SERVER_H

#endif // SERVER_H