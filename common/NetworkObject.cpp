#include "NetworkObject.hpp"

int NetworkObject::nextNetworkId = 0;
NetworkObject::NetworkObject() { _networkId = nextNetworkId++; }