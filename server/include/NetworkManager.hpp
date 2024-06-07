#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#include <vector>
#include <stack>
#include <deque>
#include <mutex>
#include "Packet.hpp"
#include "Entity.hpp"
#include "NetworkObject.hpp"
#include "engine/Scene.hpp"
class NetworkManager {
private:
    std::mutex _mutex;
    std::deque<std::pair<int, Packet*>> message_queue;

public:
    Scene scene;
    int numAlive = 0;
    void init();
    void update(float deltaTime);
    void send_state();
    void process_input();
    void send_next_phase();
    void send_combo(int clientId, int comboIndex);
    void on_message_received(const EventArgs* e);
    void on_client_joined(const EventArgs* e);
    static NetworkManager& instance() {
        static NetworkManager s;
        return s;
    }
};

#endif // NETWORK_MANAGER_H