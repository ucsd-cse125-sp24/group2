#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include "MessageBroker.hpp"


class IComponent {

public:
    virtual void receive(void* value); // NOTE: can't make this = 0 because it breaks other stuff :(
    virtual std::string toString();

    void sendMessage(std::string topic, void* value);
    void subscribe(std::string topic);
    void unsub(std::string topic);
    void update();
};


#endif // COMPONENT_H