#include "../include/Component.hpp"

#include <iostream>
#include <string>


// FUNCTION DEFINITIONS
void IComponent::receive(Topic topic, void* value) {
    std::cerr << "IComponent::recieve() should not be overriden, not called" << std::endl;
    return;
}

void IComponent::sendMessage(Topic topic, void* value) {
    MessageBroker::instance().broadcast(topic, value);
}

void IComponent::subscribe(Topic topic) {
    MessageBroker::instance().addSub(this, topic);
}

void IComponent::unsub(Topic topic) {
    MessageBroker::instance().removeSub(this, topic);
}

std::string IComponent::toString() {
    return "ERROR: IComponent::toString() should be overriden";
}