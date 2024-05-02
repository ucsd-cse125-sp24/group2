#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#include <vector>
#include <stack>
#include <deque>
#include <mutex>
#include "Packet.hpp"
#include "Entity.hpp"
class NetworkManager {
    std::vector<Entity*> entities;
    static std::stack<int> freelist;

private:
    std::mutex _mutex;
    std::deque<std::pair<int, Packet*>> message_queue;

public:
    static int next_network_id;
    void init();
    void update();
    void send_state();
    void process_input();
    void register_entity(Entity*);
    void unregister_entity(Entity*);
    void handle_packet(int, void*);
    void handle_input(Packet* packet, int client_id);
    static NetworkManager& instance() {
        static NetworkManager s;
        return s;
    }
};

#endif // NETWORK_MANAGER_H