#include "../include/INetworkComponent.hpp"

void INetworkComponent::Serialize(Packet*) {}

void INetworkComponent::Deserialize(Packet*) {}

std::string INetworkComponent::ToString() {
    return "INetworkComponent";
}