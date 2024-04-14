#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#include <vector>
#include <stack>

#include "Entity.hpp"
class NetworkManager {
    std::vector<Entity*> entities;
    static std::stack<int> freelist;

   public:
    void init();
    void update();
    void send_state();
    void register_entity(Entity*);
    static NetworkManager& instance() {
        static NetworkManager s;
        return s;
    }
};

#endif  // NETWORK_MANAGER_H