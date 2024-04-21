#include "../include/Component.hpp"

#include <iostream>
#include <string>


// FUNCTION DEFINITIONS
void IComponent::receive(void* value) {
    std::cerr << "IComponent::recieve() should not be overriden, not called" << std::endl;
    return;
}

void IComponent::sendMessage(std::string topic, void* value) {
    MessageBroker::instance().broadcast(topic, value);
}

void IComponent::subscribe(std::string topic) {
    MessageBroker::instance().addSub(this, topic);
}

void IComponent::unsub(std::string topic) {
    MessageBroker::instance().removeSub(this, topic);
}