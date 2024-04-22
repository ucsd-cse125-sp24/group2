#ifndef MESSAGE_BROKER_H
#define MESSAGE_BROKER_H

#include "Component.hpp"

#include <mutex>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <iostream>

class IComponent;


// Topic enumerated list, add to this if you want to make a new topic

enum Topic { testPosition, testRotation, testScale };

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