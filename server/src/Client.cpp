#include "Client.hpp"

void Client::init() {}

void Client::disconnect() {
    delete clientsock;
    clientsock = nullptr;
}

/*
 * Track objects being replicated per-client
 */
void Client::track_object(NetworkObject* object) {
    int objId = object->networkId();
    objectStates[objId].status = NetworkObjectState::ON_SERVER;
    if (objectStates.find(objId) != objectStates.end()) {
        return;
    }

    NetworkObjectState objectState;
    objectStates[object->networkId()] = objectState;
}