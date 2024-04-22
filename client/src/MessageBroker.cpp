#include "../include/MessageBroker.hpp"


// FUNCTION DEFINITIONS

/**
 * Notifies all subscribers to a topic of a new value
*/
void MessageBroker::broadcast(Topic topic, void* value) {
    if (topicToSubsMap.find(topic) == topicToSubsMap.end()) {
        std::cerr << "No such topic to broadcast to." << std::endl;
        return;
    }

    std::unordered_set<IComponent*> subscribers = topicToSubsMap[topic];
    for (IComponent* sub : subscribers)
        sub->receive(topic, value);
}

/**
 * Adds an IComponent to topic
*/
void MessageBroker::addSub(IComponent* newSub, Topic topic) {
    if (topicToSubsMap.find(topic) == topicToSubsMap.end()) {  // if this is a new topic
        std::unordered_set<IComponent*> subscribers;
        subscribers.insert(newSub);
        topicToSubsMap.emplace(topic, subscribers);          
    } else {  // otherwise
        topicToSubsMap[topic].insert(newSub);
    }
}

/**
 * Removes an IComponent from topic
*/
void MessageBroker::removeSub(IComponent* sub, Topic topic) {
    if (topicToSubsMap.find(topic) == topicToSubsMap.end()) {
        std::cerr << "No such topic to removeSub from." << std::endl;
        return;
    }
    
    topicToSubsMap[topic].erase(sub);
}
