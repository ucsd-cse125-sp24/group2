#ifndef NETWORK_OBJECT_H
#define NETWORK_OBJECT_H


#include "GameObject.hpp"
#include "Packet.hpp"
#include "INetworkComponent.hpp"


class INetworkComponent;

enum NetworkObjectTypeID : int32_t {
    PLAYER,
};

class NetworkObject : public GameObject {
protected:
    int _networkId;
    static int nextNetworkId;
    std::vector<INetworkComponent*> networkComponents;

public:
    // Constructors

    NetworkObject();

    // Overriden functions from GameObject

    void AddComponent(IComponent* newComp) override;
    void RemoveComponent(IComponent* comp) override;

    // New functions

    virtual void serialize(Packet*) = 0; // serialize order needs to match deserialize order, and vice versa
    virtual void deserialize(Packet*) = 0;
    virtual int32_t TypeID() const = 0;
    inline int networkId() const { return _networkId; }

    // std::string ToString() override;
};


#endif