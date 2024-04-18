#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#include <vector>
#include <stack>
#include <deque>
#include <mutex>

#include "Entity.hpp"
class NetworkManager {
    std::vector<Entity*> entities;
    static std::stack<int> freelist;

private:
    std::mutex _mutex;
    // TODO change to Packet*
    std::deque<uint8_t*> message_queue;

public:
    void init();
    void update();
    void send_state();
    void process_input();
    void register_entity(Entity*);
    void handle_packet(void*);
    static NetworkManager& instance() {
        static NetworkManager s;
        return s;
    }
};

#endif // NETWORK_MANAGER_H