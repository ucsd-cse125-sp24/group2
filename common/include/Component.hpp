#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>


class IComponent {
public:
    bool sendMessage(std::string topic, void* value);
    bool notify(void* value);
    bool subscribe(std::string topic);
    bool unsub(std::string topic);
    bool update();
};

#endif // COMPONENT_H