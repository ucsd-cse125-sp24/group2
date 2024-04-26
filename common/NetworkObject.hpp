#ifndef NETWORK_OBJECT_H
#define NETWORK_OBJECT_H
#include "GameObject.hpp"
#include "Packet.hpp"
class NetworkObject : public GameObject {
public:
    int networkId;
    virtual void serialize(Packet*) = 0;
    virtual void deserialize(Packet*) = 0;
};
#endif