#ifndef EVENT_ARGS_H
#define EVENT_ARGS_H
#include <iomanip>
#include <iostream>
typedef struct EventArgs {
} EventArgs;

typedef struct MessageReceivedEventArgs : public EventArgs {
    int fromClient;
    uint8_t* buffer;
    int size;

    MessageReceivedEventArgs(int fromClient, uint8_t* buffer, int size)
        : fromClient(fromClient), buffer(buffer), size(size) {}
} MessageReceivedEventArgs;

typedef struct ClientJoinedEventArgs : public EventArgs {
    int clientId;

    ClientJoinedEventArgs(int clientId) : clientId(clientId) {}
} ClientJoinedEventArgs;

typedef struct ObjectEventArgs : public EventArgs {
    Entity* e;

    ObjectEventArgs(Entity* e) : e(e) {}
} ObjectEventArgs;
#endif