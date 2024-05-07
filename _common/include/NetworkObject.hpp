#ifndef NETWORK_OBJECT_H
#define NETWORK_OBJECT_H

#include "GameObject.hpp"
#include "Packet.hpp"
#include "INetworkComponent.hpp"

class INetworkComponent;

class NetworkObject : public GameObject {
private:
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

    virtual void serialize(Packet*) = 0;
    virtual void deserialize(Packet*) = 0;
    inline int networkId() const { return _networkId; }

    // std::string ToString() override;
};

#endif