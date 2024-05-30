#ifndef EVENT_ARGS_H
#define EVENT_ARGS_H
#include <iomanip>
#include <iostream>
#include "Entity.hpp"
typedef struct EventArgs {
} EventArgs;

typedef struct MessageReceivedEventArgs : public EventArgs {
    int fromClient;
    uint8_t* buffer;
    int size;

    MessageReceivedEventArgs(int fromClient, uint8_t* buffer, int size)
        : fromClient(fromClient), buffer(buffer), size(size) {}
} MessageReceivedEventArgs;

typedef struct ClientEventArgs : public EventArgs {
    int clientId;

    ClientEventArgs(int clientId) : clientId(clientId) {}
} ClientEventArgs;

typedef struct ObjectEventArgs : public EventArgs {
    Entity* e;

    ObjectEventArgs(Entity* e) : e(e) {}
} ObjectEventArgs;

typedef struct DestroyedEventArgs : public EventArgs {
    std::vector<int> destroyedObjectIds;

    DestroyedEventArgs(std::vector<int> destroyedObjectIds)
        : destroyedObjectIds(destroyedObjectIds) {}
} DestroyedEventArgs;

typedef struct BeatEventArgs : public EventArgs {
    int beat;

    BeatEventArgs(int beat) : beat(beat) {}
} BeatEventArgs;
#endif