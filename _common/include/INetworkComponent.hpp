#ifndef I_NETWORK_COMPONENT_H
#define I_NETWORK_COMPONENT_H


#include "IComponent.hpp"
#include "Packet.hpp"


class INetworkComponent : public IComponent {

public:
    void Serialize(Packet*);
    void Deserialize(Packet*);
};


#endif // I_NETWORK_COMPONENT_H