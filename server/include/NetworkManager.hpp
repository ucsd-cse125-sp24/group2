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
    std::deque<Packet*> message_queue;

public:
    void init();
    void update();
    void send_state();
    void process_input();
    void register_entity(Entity*);
    void unregister_entity(Entity*);
    void handle_packet(void*);
    static NetworkManager& instance() {
        static NetworkManager s;
        return s;
    }
};

#endif // NETWORK_MANAGER_H