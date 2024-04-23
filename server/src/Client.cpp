#include "Client.hpp"

void Client::init() { p = new Player; }

void Client::disconnect() {
    /*
      delete p;
      p = nullptr;
      */
    delete clientsock;
    clientsock = nullptr;
}