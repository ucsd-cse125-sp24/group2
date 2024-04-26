#ifndef NETWORK_OBJECT_H
#define NETWORK_OBJECT_H
#include "GameObject.hpp"
#include "Packet.hpp"
class NetworkObject : public GameObject {
private:
    int _networkId;
    static int nextNetworkId;

public:
    NetworkObject();
    virtual void serialize(Packet*) = 0;
    virtual void deserialize(Packet*) = 0;
    inline int networkId() const { return _networkId; }
};
#endif