#include "NetworkObject.hpp"
#include <algorithm>
#include <iostream>

#include "NetTransform.hpp"
#include "Mover.hpp"
#include "MovementStateMachine.hpp"
#include "Health.hpp"
#include "Status.hpp"

int NetworkObject::nextNetworkId = 0;

NetworkObject::NetworkObject() : GameObject() { _networkId = nextNetworkId++; }
NetworkObject::NetworkObject(int networkId) : GameObject() {
    _networkId = networkId;
}

void NetworkObject::AddComponent(IComponent* newComp) {
    if (INetworkComponent* newNetComp =
            dynamic_cast<INetworkComponent*>(newComp)) {
        networkComponents.push_back(newNetComp);
        typeToComponentMap.emplace(typeid(*newComp), newComp);
    } else {
        components.push_back(newComp);
        typeToComponentMap.emplace(typeid(*newComp), newComp);
    }
    newComp->SetOwner(this);
}

void NetworkObject::RemoveComponent(IComponent* comp) {
    if (INetworkComponent* netComp = dynamic_cast<INetworkComponent*>(comp)) {
        std::remove(networkComponents.begin(), networkComponents.end(),
                    netComp);
        typeToComponentMap.erase(typeid(*comp));
    } else {
        std::remove(components.begin(), components.end(), comp);
        typeToComponentMap.erase(typeid(*comp));
    }
    comp->SetOwner(nullptr);
}

void NetworkObject::serialize(Packet* packet) {
    packet->write_int(_networkId);
    for (INetworkComponent* netComp : networkComponents) {
        packet->write_int(netComp->TypeID());
        netComp->Serialize(packet);
    }
}

// NOTE: make sure to extend to support all descendants of INetworkComponents
void NetworkObject::deserialize(Packet* packet) {
    for (int i = 0; i < networkComponents.size(); ++i) {
        int32_t compTypeID;
        packet->read_int(&compTypeID);

        switch (compTypeID) {
        case NetworkComponentTypeID::TRANSFORM: {
            NetTransform* transform = GetComponent<NetTransform>();
            if (transform == nullptr)
                std::cout << "ERROR in NetworkObject::deserialize(): No "
                             "NetTransform found in current NetworkObject"
                          << std::endl;
            transform->Deserialize(packet);
            break;
        }
        case NetworkComponentTypeID::MOVER: {
            Mover* mover = GetComponent<Mover>();
            if (mover == nullptr)
                std::cout << "ERROR in NetworkObject::deserialize(): No Mover "
                             "found in current NetworkObject"
                          << std::endl;
            mover->Deserialize(packet);
            break;
        }
        case NetworkComponentTypeID::HEALTH: {
            Health* health = GetComponent<Health>();
            if (health == nullptr)
                std::cout << "ERROR in NetworkObject::deserialize(): No Health "
                             "found in current NetworkObject"
                          << std::endl;
            health->Deserialize(packet);
            break;
        }
        case NetworkComponentTypeID::MOVEMENT_STATE_MACHINE: {
            MovementStateMachine* movementStateMachine = GetComponent<MovementStateMachine>();
            if (!movementStateMachine) {
                std::cout << "ERROR in NetworkObject::deserialize(): No "
                             "MovementStateMachine found in current NetworkObject"
                          << std::endl;
            }
            movementStateMachine->Deserialize(packet);
            break;
        }
        case NetworkComponentTypeID::STATUS: {
            Status* status = GetComponent<Status>();
            if (!status) {
                std::cout << "ERROR in NetworkObject::deserialize(): No "
                             "MovementStateMachine found in current NetworkObject"
                          << std::endl;
            }
            status->Deserialize(packet);
            break;
        }
        default: {
            std::cout << "      ERROR in NetworkObject::deserialize(): TypeID: "
                      << compTypeID
                      << " not currently supported in "
                         "NetworkObject::deserialize(Packet*)"
                      << std::endl;
            break;
        }
        }
    }
}