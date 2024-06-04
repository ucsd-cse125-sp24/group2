#ifndef I_NETWORK_COMPONENT_H
#define I_NETWORK_COMPONENT_H


#include "IComponent.hpp"
#include "Packet.hpp"
#include "NetworkObject.hpp"


class NetworkObject;

enum NetworkComponentTypeID : int32_t {
    TRANSFORM = 8,
    MOVER = 9,
    HEALTH = 10,
    ENEMY_COMPONENT,
    SKILL_TYPE
};

class INetworkComponent : public IComponent {

public:
    INetworkComponent() : 
        IComponent()
    {}
    
    INetworkComponent(NetworkObject* owner) :
        IComponent((GameObject*)owner)
    {}

    virtual void Serialize(Packet*) = 0;
    virtual void Deserialize(Packet*) = 0;
    virtual int32_t TypeID() const = 0;

    virtual std::string ToString() override;
};


#endif // I_NETWORK_COMPONENT_H