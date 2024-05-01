#ifndef NETWORK_OBJECT_STATE_H
#define NETWORK_OBJECT_STATE_H
#include "NetworkObject.hpp"

struct NetworkObjectState {
    enum StatusFlags : uint8_t {
        UNUSED = 0, // for object pooling
        ON_SERVER = 1,
        ON_CLIENT = 2,
        AWAITING_DESTRUCTION = 4,
        DESTROYING = 8
    } status;

    inline bool valid_on_server() const { return (status & ON_CLIENT); }
    inline bool valid_on_client() const {
        return ((status & (AWAITING_DESTRUCTION | DESTROYING)) == 0) &&
               status != UNUSED;
    }
    inline bool validOnBoth() const {
        return valid_on_client() && valid_on_server();
    }
};
#endif