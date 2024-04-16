#ifndef CLIENT_H
#define CLIENT_H

#include "psocket.hpp"
#include <functional>
#include <mutex>

typedef std::function<void(void*)> ReceiveHandler;
class Client {
private:
public:
    std::mutex mutex;
    Socket psocket;
    void connect(const char*, uint16_t);
    static void* receive(void*);
    void send(const char*, int);
    ReceiveHandler receive_event = nullptr;
    void setCallback(const ReceiveHandler& callback);
};
#endif // CLIENT_H