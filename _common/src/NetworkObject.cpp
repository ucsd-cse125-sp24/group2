#include "NetworkObject.hpp"
#include <algorithm>

int NetworkObject::nextNetworkId = 0;

NetworkObject::NetworkObject() : GameObject() { _networkId = nextNetworkId++; }

void NetworkObject::AddComponent(IComponent* newComp) {
    if (INetworkComponent* newNetComp =
            dynamic_cast<INetworkComponent*>(newComp)) {
        networkComponents.push_back(newNetComp);
        typeToComponentMap.emplace(typeid(*newComp), newComp);
    } else {
        components.push_back(newComp);
        typeToComponentMap.emplace(typeid(*newComp), newComp);
    }
}

void NetworkObject::RemoveComponent(IComponent* comp) {
    if (INetworkComponent* netComp = dynamic_cast<INetworkComponent*>(comp)) {
        std::remove(networkComponents.begin(), networkComponents.end(),
                    netComp);
        typeToComponentMap.erase(typeid(*comp));
    } else {
        // components.push_back(comp);
        std::remove(components.begin(), components.end(), comp);
        typeToComponentMap.erase(typeid(*comp));
    }
}

void NetworkObject::serialize(Packet* packet) {
    for (INetworkComponent* netComp : networkComponents) {
        netComp->Serialize(packet);
    }
}

void NetworkObject::deserialize(Packet* packet) {
    for (INetworkComponent* netComp : networkComponents) {
        netComp->Deserialize(packet);
    }
}