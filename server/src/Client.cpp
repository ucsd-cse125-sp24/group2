#include "Client.hpp"

void Client::init() {}

void Client::disconnect() {
    delete clientsock;
    clientsock = nullptr;
}