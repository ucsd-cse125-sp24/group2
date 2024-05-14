#include "NetworkObject.hpp"
#include <algorithm>
#include <iostream>

#include "NetTransform.hpp"
#include "Mover.hpp"


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
    // std::cout << "    NetObj::serialize()" << std::endl;
    
    // std::cout << "      NetID: " << _networkId << std::endl;

    packet->write_int(_networkId);
    for (INetworkComponent* netComp : networkComponents) {

        // std::cout << "      Serializing: " << netComp->TypeID() << std::endl;

        packet->write_int(netComp->TypeID());
        netComp->Serialize(packet);
    }
}

// NOTE: make sure to extend to support all descendants of INetworkComponents
void NetworkObject::deserialize(Packet* packet) {
    for (int i = 0; i < networkComponents.size(); ++i) {
        int32_t compTypeID;
        packet->read_int(&compTypeID);

        // std::cout << "      Deserializing: ";

        switch (compTypeID) {
            case NetworkComponentTypeID::TRANSFORM: {

                // std::cout << "TRANSFORM" << std::endl;

                NetTransform* transform = GetComponent<NetTransform>();
                if (transform == nullptr) std::cout << "ERROR in NetworkObject::deserialize(): No NetTransform found in current NetworkObject" << std::endl;
                transform->Deserialize(packet);
                break;
            }
            case NetworkComponentTypeID::MOVER: {

                // std::cout << "MOVER" << std::endl;

                Mover* mover = GetComponent<Mover>();
                if (mover == nullptr) std::cout << "ERROR in NetworkObject::deserialize(): No Mover found in current NetworkObject" << std::endl;
                mover->Deserialize(packet);
                break;
            }
            default: {

                // std::cout << "NONEOFTHEABOVE" << std::endl;

                std::cout << "      ERROR in NetworkObject::deserialize(): TypeID: " << compTypeID << " not currently supported in NetworkObject::deserialize(Packet*)" << std::endl;
                break;
            }
        }
    }
    // for (INetworkComponent* netComp : networkComponents) {
    //     netComp->Deserialize(packet);
    // }
}