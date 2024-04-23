#ifndef MESSAGE_BROKER_H
#define MESSAGE_BROKER_H

#include "Component.hpp"

#include <mutex>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <iostream>
#include "Topic.hpp"

class IComponent;


// MessageBroker Singleton

class MessageBroker {
    std::unordered_map<Topic, std::unordered_set<IComponent*>> topicToSubsMap;

public:
    void broadcast(Topic topic, void* value);
    void addSub(IComponent* newSub, Topic topic);
    void removeSub(IComponent* sub, Topic topic);
    
    static MessageBroker& instance() {
        static MessageBroker s;
        return s;
    }
};


#endif // MESSAGE_BROKER_H