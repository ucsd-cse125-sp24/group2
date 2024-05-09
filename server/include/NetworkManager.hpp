#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#include <vector>
#include <stack>
#include <deque>
#include <mutex>
#include "Packet.hpp"
#include "Entity.hpp"
#include "NetworkObject.hpp"
#include "EventArgs.hpp"
class NetworkManager {
private:
    std::mutex _mutex;
    std::deque<std::pair<int, Packet*>> message_queue;

public:
    void init();
    void update();
    void send_state();
    void process_input();
    void on_message_received(const EventArgs* e);
    void on_client_joined(const EventArgs* e);
    void handle_input(Packet* packet, int client_id);
    static NetworkManager& instance() {
        static NetworkManager s;
        return s;
    }
};

#endif // NETWORK_MANAGER_H