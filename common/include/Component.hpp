#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include "MessageBroker.hpp"


class IComponent {

public:
    virtual void receive(void* value) = 0; // NOTE: can't make this = 0 because it breaks other stuff :(

    void sendMessage(std::string topic, void* value);
    void subscribe(std::string topic);
    void unsub(std::string topic);
    void update();
};


// FUNCTION DEFINITIONS

void IComponent::sendMessage(std::string topic, void* value) {
    MessageBroker::instance().broadcast(topic, value);
}

void IComponent::subscribe(std::string topic) {
    MessageBroker::instance().addSub(this, topic);
}

void IComponent::unsub(std::string topic) {
    MessageBroker::instance().removeSub(this, topic);
}


#endif // COMPONENT_H