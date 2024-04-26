#ifndef EVENT_ARGS_H
#define EVENT_ARGS_H
#include <iomanip>
#include <iostream>
typedef struct EventArgs {
} EventArgs;

typedef struct MessageReceivedEventArgs : public EventArgs {
    int fromClient;
    uint8_t* buffer;

    MessageReceivedEventArgs(int fromClient, uint8_t* buffer)
        : fromClient(fromClient), buffer(buffer) {}
} MessageReceivedEventArgs;

typedef struct ClientJoinedEventArgs : public EventArgs {
    int clientId;

    ClientJoinedEventArgs(int clientId) : clientId(clientId) {}
} ClientJoinedEventArgs;
#endif