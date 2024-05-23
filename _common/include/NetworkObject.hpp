#ifndef NETWORK_OBJECT_H
#define NETWORK_OBJECT_H

#include "GameObject.hpp"
#include "Packet.hpp"
#include "INetworkComponent.hpp"

class INetworkComponent;

enum NetworkObjectTypeID : int32_t { PLAYER, ENEMY, PLAYER_ATTACK, ENEMY_ATTACK };
class NetworkObject : public GameObject {
protected:
    int _networkId; // to distinguish each NetworkObject
    static int nextNetworkId;
    std::vector<INetworkComponent*>
        networkComponents; // container for all INetworkComponents, disjoint
                           // from GameObject::components

public:
    // Constructors

    NetworkObject();
    NetworkObject(int networkId);

    // Overriden functions from GameObject

    void AddComponent(
        IComponent* newComp) override; // Adds component to either components or
                                       // networkComponents based on type
    void RemoveComponent(
        IComponent* comp) override; // Removes component from either components
                                    // or networkComponents based on type

    // New functions

    void serialize(Packet*); // Uses INetworkComponent::TypeID(), so make sure
                             // to implement for each INetworkComponent
    void
    deserialize(Packet*); // Needs to know all possible INetworkComponents, make
                          // sure to extend function to support each one created
    virtual int32_t
    TypeID() const = 0; // Used to determine size of object in deserialization,
                        // make sure to implement for each NetworkObject
    inline int networkId() const { return _networkId; }

    // std::string ToString() override;
};

#endif