#ifndef CLIENT_H
#define CLIENT_H

#include "psocket.hpp"
#include <functional>
#include <mutex>
#include "Packet.hpp"

typedef std::function<void(Packet*)> ReceiveHandler;
class Client {
private:
public:
    std::mutex mutex;
    Socket psocket;
    Packet receivedData;
    void connect(const char*, uint16_t);
    void receive(void);
    void send(Packet*);
    ReceiveHandler receive_event = nullptr;
    void setCallback(const ReceiveHandler& callback);
};
#endif // CLIENT_H