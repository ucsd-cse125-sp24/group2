#include "../include/MessageBroker.hpp"


/**
 * Notifies all subscribers to a topic of a new value
*/
void MessageBroker::broadcast(std::string topic, void* value) {
    std::vector<IComponent> subscribers;
    for (IComponent sub : subscribers)
        sub.receive(value);
}

/**
 * Adds an IComponent to topic
*/
void MessageBroker::addSub(IComponent newSub, std::string topic) {
    if (topicToSubMap.find(topic) == topicToSubMap.end()) {
        std::vector<IComponent> subscribers;
        subscribers.push_back(newSub);
        topicToSubMap.emplace(topic, subscribers);
    } else {
        
    }
}
