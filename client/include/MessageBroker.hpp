#ifndef MESSAGE_BROKER_H
#define MESSAGE_BROKER_H

#include "Component.hpp"

#include <mutex>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <iostream>

class IComponent;


class MessageBroker {
    std::unordered_map<std::string, std::unordered_set<IComponent*>> topicToSubsMap;


public:
    void broadcast(std::string topic, void* value);
    void addSub(IComponent* newSub, std::string topic);
    void removeSub(IComponent* sub, std::string topic);
    
    static MessageBroker& instance() {
        static MessageBroker s;
        return s;
    }
};


#endif // MESSAGE_BROKER_H