#ifndef I_NETWORK_COMPONENT_H
#define I_NETWORK_COMPONENT_H


#include "IComponent.hpp"
#include "Packet.hpp"
#include "NetworkObject.hpp"


class NetworkObject;


class INetworkComponent : public IComponent {
    
protected:
    NetworkObject* netOwner;

public:
    INetworkComponent() : 
        IComponent(), 
        netOwner(nullptr) 
    {}
    
    INetworkComponent(NetworkObject* newNetOwner) :
        IComponent((GameObject*)newNetOwner),
        netOwner(newNetOwner)
    {}

    virtual void Serialize(Packet*);
    virtual void Deserialize(Packet*);
};


#endif // I_NETWORK_COMPONENT_H