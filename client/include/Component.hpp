#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include "MessageBroker.hpp"

class IComponent {

public:
    virtual void receive(Topic topic, void* value); // NOTE: can't make this = 0 because it breaks other stuff :(
    virtual std::string toString();

    void sendMessage(Topic topic, void* value);
    void subscribe(Topic topic);
    void unsub(Topic topic);
    void update();
};


#endif // COMPONENT_H