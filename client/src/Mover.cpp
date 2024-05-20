#include "Mover.hpp"
#include "NetTransform.hpp"

Mover::Mover(NetworkObject* owner)
    : INetworkComponent(owner),
      position(owner->GetComponent<NetTransform>()->position),
      rotation(owner->GetComponent<NetTransform>()->rotation) {}

void Mover::Update() {}

std::string Mover::ToString() { return "Mover"; }