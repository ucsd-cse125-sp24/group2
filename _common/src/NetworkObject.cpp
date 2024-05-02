#include "NetworkObject.hpp"

int NetworkObject::nextNetworkId = 0;

NetworkObject::NetworkObject() : GameObject() { 
    _networkId = nextNetworkId++; 
}