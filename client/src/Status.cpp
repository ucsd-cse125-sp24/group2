#include "Status.hpp"
#include "Mover.hpp"


Status::Status(NetworkObject* owner) : 
    INetworkComponent(owner),
    baseSpeed(owner->GetComponent<Mover>()->baseSpeed) // TODO: Update this to be mover->baseSpeed when branch "dodging" is merged in
{}