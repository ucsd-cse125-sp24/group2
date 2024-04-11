#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#include <vector>
#include <string>
#include <stack>
#include <stdio.h>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include "entity.hpp"
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